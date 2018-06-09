/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */
#include <utility>
#include <vector>
#include <string>
#include <eosiolib/eosio.hpp>
#include <eosiolib/time.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/contract.hpp>
#include <eosiolib/crypto.h>
#include <map>

using eosio::action;
using eosio::asset;
using eosio::const_mem_fun;
using eosio::indexed_by;
using eosio::key256;
using eosio::name;
using eosio::permission_level;
using eosio::print;

class dice : public eosio::contract
{
public:
  const uint32_t FIVE_MINUTES = 5 * 60;
  const uint32_t ONE_DAY = 24 * 60 * 60;

  dice(account_name self)
      : eosio::contract(self),
        offers(_self, _self),
        games(_self, _self),
        global_dices(_self, _self),
        red_packages(_self, _self),
        accounts(_self, _self)
  {
  }
  void sent(const account_name sender, const asset &total_amount, const uint64_t people_limit,
            const std::string &description, const uint64_t lat, const uint64_t lng, const uint64_t r1)
  {
    require_auth(sender);
    eosio_assert(total_amount.is_valid(), "invalid bet");
    eosio_assert(total_amount.amount > 0, "must bet positive quantity");

    eosio_assert(people_limit > 0, "must bet positive quantity");

    // Create global red package counter if not exists
    auto gdice_itr = global_dices.begin();
    if (gdice_itr == global_dices.end())
    {
      gdice_itr = global_dices.emplace(_self, [&](auto &gdice) {
        gdice.nextgameid = 0;
      });
    }

    // Increment global red package counter
    global_dices.modify(gdice_itr, 0, [&](auto &gdice) {
      gdice.nextgameid++;
    });

    // Create a new red package
    auto itr = red_packages.emplace(_self, [&](auto &package) {
      package.id = gdice_itr->nextgameid; // !
      package.total_amount = total_amount;
      package.people_limit = people_limit;
      package.description = description;
      package.lat = lat;
      package.lng = lng;
      package.r1 = r1;
      package.deadline = eosio::time_point_sec(now() + ONE_DAY);
    });
    action(
        permission_level{sender, N(active)},
        N(eosio.token), N(transfer),
        std::make_tuple(sender, _self, total_amount, std::string("")))
        .send();
  }
  void take(const account_name taker, const uint64_t red_package_id)
  {
    require_auth(taker);
    auto itr = red_packages.find(red_package_id);
    eosio_assert(itr != red_packages.end(), "This Red Envelope is not exist.");
    eosio_assert(itr->ledger_account.size() < itr->people_limit, "This Red Envelope is empty.");
    red_packages.modify(itr, 0, [&](auto &package) {
      asset amount = package.take(taker);
      action(
          permission_level{_self, N(active)},
          N(eosio.token), N(transfer),
          std::make_tuple(_self, taker, amount, std::string("")))
          .send();
    });
  }

  //@abi action
  void add_whitelist(const account_name sender, const uint64_t red_package_id, const account_name target)
  {
    require_auth(sender);
    auto itr = red_packages.find(red_package_id);    
    eosio_assert(itr != red_packages.end(), "This Red Envelope is not exist.");
    eosio_assert(itr->sender == sender, "This red package does not belongs to you.");    

    red_packages.modify(itr, 0, [&](auto &package) {
      package.whitelist.push_back(target);
    });    
  }  
  //@abi action
  void offerbet(const asset &bet, const account_name player, const checksum256 &commitment)
  {
  }


  //@abi action
  void reveal(const checksum256 &commitment, const checksum256 &source)
  {
  }

  //@abi action
  void claimexpired(const account_name sender, const uint64_t red_package_id)
  {
    require_auth(sender);    
    auto itr = red_packages.find(red_package_id);
    eosio_assert(itr != red_packages.end(), "Red package not found.");
    eosio_assert(itr->sender == sender, "This red package does not belongs to you.");    
    eosio_assert(itr->deadline != eosio::time_point_sec(0) && eosio::time_point_sec(now()) > itr->deadline, "Red package not expired");
    // ...
  }

  //@abi action
  void deposit(const account_name from, const asset &quantity)
  {
  }

  //@abi action
  void withdraw(const account_name to, const asset &quantity)
  {
  }

private:
  //@abi table offer i64
  struct offer
  {
    uint64_t id;
    account_name owner;
    asset bet;
    checksum256 commitment;
    uint64_t gameid = 0;

    uint64_t primary_key() const { return id; }

    uint64_t by_bet() const { return (uint64_t)bet.amount; }

    key256 by_commitment() const { return get_commitment(commitment); }

    static key256 get_commitment(const checksum256 &commitment)
    {
      const uint64_t *p64 = reinterpret_cast<const uint64_t *>(&commitment);
      return key256::make_from_word_sequence<uint64_t>(p64[0], p64[1], p64[2], p64[3]);
    }

    EOSLIB_SERIALIZE(offer, (id)(owner)(bet)(commitment)(gameid))
  };

  typedef eosio::multi_index<N(offer), offer,
                             indexed_by<N(bet), const_mem_fun<offer, uint64_t, &offer::by_bet>>,
                             indexed_by<N(commitment), const_mem_fun<offer, key256, &offer::by_commitment>>>
      offer_index;

  //@abi table red i64
  struct red
  {
    uint64_t id;
    account_name sender;                      // Sender
    asset total_amount;                       // 总金额
    uint64_t people_limit;                    // 总人数
    std::string description;                  // 红包描述
    std::vector<account_name> ledger_account; // 红包账本
    std::vector<asset> ledger_asset;          // ..
    eosio::time_point_sec deadline;           // 撤回
    std::vector<account_name> whitelist;

    uint64_t lat, lng;
    uint64_t r1, r2;

    uint64_t primary_key() const { return id; }
    asset take(account_name taker)
    {
      require_auth(taker);
      for (auto i : ledger_account)
      {
        eosio_assert(i != taker, "Already take one!");
      }
      ledger_account.push_back(taker);
      ledger_asset.push_back(total_amount);
      ledger_asset.back() /= people_limit;
      return ledger_asset.back();
    }
    EOSLIB_SERIALIZE(red, (id)(sender)(total_amount)(people_limit)(description)(ledger_account)(ledger_asset)(deadline)(whitelist)(lat)(lng)(r1)(r2))
  };
  typedef eosio::multi_index<N(red), red> red_packages_index;

  struct player
  {
    checksum256 commitment;
    checksum256 reveal;

    EOSLIB_SERIALIZE(player, (commitment)(reveal))
  };

  //@abi table game i64
  struct game
  {
    uint64_t id;
    asset bet;
    eosio::time_point_sec deadline;
    player player1;
    player player2;

    uint64_t primary_key() const { return id; }

    EOSLIB_SERIALIZE(game, (id)(bet)(deadline)(player1)(player2))
  };

  typedef eosio::multi_index<N(game), game> game_index;

  //@abi table global i64
  struct global_dice
  {
    uint64_t id = 0;
    uint64_t nextgameid = 0;

    uint64_t primary_key() const { return id; }

    EOSLIB_SERIALIZE(global_dice, (id)(nextgameid))
  };

  typedef eosio::multi_index<N(global), global_dice> global_dice_index;

  //@abi table account i64
  struct account
  {
    account(account_name o = account_name()) : owner(o) {}

    account_name owner;
    asset eos_balance;
    uint32_t open_offers = 0;
    uint32_t open_games = 0;

    bool is_empty() const { return !(eos_balance.amount | open_offers | open_games); }

    uint64_t primary_key() const { return owner; }

    EOSLIB_SERIALIZE(account, (owner)(eos_balance)(open_offers)(open_games))
  };

  typedef eosio::multi_index<N(account), account> account_index;

  offer_index offers;
  game_index games;
  global_dice_index global_dices;
  account_index accounts;
  red_packages_index red_packages;

  bool has_offer(const checksum256 &commitment) const
  {
    auto idx = offers.template get_index<N(commitment)>();
    auto itr = idx.find(offer::get_commitment(commitment));
    return itr != idx.end();
  }

  bool is_equal(const checksum256 &a, const checksum256 &b) const
  {
    return memcmp((void *)&a, (const void *)&b, sizeof(checksum256)) == 0;
  }

  bool is_zero(const checksum256 &a) const
  {
    const uint64_t *p64 = reinterpret_cast<const uint64_t *>(&a);
    return p64[0] == 0 && p64[1] == 0 && p64[2] == 0 && p64[3] == 0;
  }

  void pay_and_clean(const game &g, const offer &winner_offer,
                     const offer &loser_offer)
  {

    // Update winner account balance and game count
    auto winner_account = accounts.find(winner_offer.owner);
    accounts.modify(winner_account, 0, [&](auto &acnt) {
      acnt.eos_balance += 2 * g.bet;
      acnt.open_games--;
    });

    // Update losser account game count
    auto loser_account = accounts.find(loser_offer.owner);
    accounts.modify(loser_account, 0, [&](auto &acnt) {
      acnt.open_games--;
    });

    if (loser_account->is_empty())
    {
      accounts.erase(loser_account);
    }

    games.erase(g);
    offers.erase(winner_offer);
    offers.erase(loser_offer);
  }
};

EOSIO_ABI(dice, (sent)(take))