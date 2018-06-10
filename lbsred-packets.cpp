/**
 *  @file
 *  @copyright MinakoKojima@Andoromeda
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
        global_dices(_self, _self),
        red_packages(_self, _self)
  {
  }
  //@abi action
  void sent(const account_name sender, const asset &total_amount, const uint64_t people_limit = 1,
            const std::string &description = "", const uint64_t lat = 0, const uint64_t lng = 0, const uint64_t r1 = 0)
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
      package.balance = total_amount;
      package.description = description;
      package.lat = lat;
      package.lng = lng;
      package.r1 = r1;
      package.deadline = eosio::time_point_sec(now() + ONE_DAY);
      package.sender = sender;
    });
    action(
        permission_level{sender, N(active)},
        N(eosio.token), N(transfer),
        std::make_tuple(sender, _self, total_amount, std::string("")))
        .send();
  }
  bool in_whitelist(const std::vector<account_name> &whitelist, const account_name target) {
    if (whitelist.empty()) return true;
    for (auto i: whitelist){
      if (i == target) return true;
    }
    return false;
  }
  //@abi action  
  void take(const account_name taker, const uint64_t red_package_id)
  {
    require_auth(taker);
    auto itr = red_packages.find(red_package_id);
    eosio_assert(itr != red_packages.end(), "This Red Envelope is not exist.");
    eosio_assert(itr->ledger_account.size() < itr->people_limit, "This Red Envelope is empty.");
    eosio_assert(in_whitelist(itr->whitelist, taker), "This user is not in the whitelist.");

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
  void addwhitelist(const account_name sender, const uint64_t red_package_id, const account_name target)
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
  void claimexpired(const account_name sender, const uint64_t red_package_id)
  {
    require_auth(sender);    
    auto itr = red_packages.find(red_package_id);
    eosio_assert(itr != red_packages.end(), "Red package not found.");
    eosio_assert(itr->sender == sender, "This red package does not belongs to you.");    
    eosio_assert(itr->deadline != eosio::time_point_sec(0) && eosio::time_point_sec(now()) > itr->deadline, "Red package not expired");

    action(
        permission_level{_self, N(active)}, N(eosio.token), N(transfer),
        std::make_tuple(_self, sender, itr->balance, std::string("")))
        .send();        
  }

private:
  //@abi table red i64
  struct red
  {
    uint64_t id;
//    account_name sender1;                      // Sender    
    asset total_amount;                       // 总金额
    asset balance;                            // Balance
    uint64_t people_limit;                    // 总人数
    std::string description;                  // 红包描述
    std::vector<account_name> ledger_account; // 红包账本
    std::vector<asset> ledger_asset;          // ..
    eosio::time_point_sec deadline;           // 撤回
    std::vector<account_name> whitelist;
 

    uint64_t lat, lng;
    uint64_t r1;

   account_name sender; // 1    

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
      balance -= ledger_asset.back();
      return ledger_asset.back();
    }

    asset take_random(account_name taker)
    { 
      require_auth(taker);
      for (auto i : ledger_account)
      {
        eosio_assert(i != taker, "Already take one!");
      }

      if (ledger_account.size() + 1 == people_limit) {
        ledger_asset.push_back(balance);
      }
      else {
        // double r = (uint64_t(sha256(std::string(eosio::time_point_sec(now())) % 10000000) / 10000000;
        ledger_asset.push_back(balance);
      }

      ledger_account.push_back(taker);
      balance -= ledger_asset.back();
      return ledger_asset.back(); 
    }

    EOSLIB_SERIALIZE(red, (id)(total_amount)(balance)(people_limit)(description)(ledger_account)(ledger_asset)(deadline)(whitelist)(lat)(lng)(r1)(sender))
  };
  typedef eosio::multi_index<N(red), red> red_packages_index;

  //@abi table global i64
  struct global_dice
  {
    uint64_t id = 0;
    uint64_t nextgameid = 0;

    uint64_t primary_key() const { return id; }

    EOSLIB_SERIALIZE(global_dice, (id)(nextgameid))
  };
  typedef eosio::multi_index<N(global), global_dice> global_dice_index;

  global_dice_index global_dices;
  red_packages_index red_packages;
};

EOSIO_ABI(dice, (sent)(take)(addwhitelist)(claimexpired))