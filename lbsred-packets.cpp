#include <eosiolib/eosio.hpp>

using namespace eosio;

class hello : public eosio::contract
{
public:
  using contract::contract;

  /// @abi action
  void hi(account_name user)
  {
    print("Hello, ", name{user});
  }

private:
  // @abi table red_packers i64
  struct red_packer
  {
    uint64_t id;
    uint64_t people;           //总人数
    uint64_t total_amount;     //总金额
    uint64_t balance;          //剩余金额
    uint64_t coordinate_range; //坐标范围多少 可以领取红包
    std::string description;   // 红包描述
    std::string coordinate;    // 红包坐标
    uint64_t completed;        // 红包状态，是否完成
    //todo:
    // whitelist
    // 每个人领取了多少

    uint64_t primary_key() const { return id; }
    EOSLIB_SERIALIZE(red_packer, (id)(description)(completed))
  };

  typedef eosio::multi_index<N(todos), red_packer> todo_table;
  todo_table todos;
};

EOSIO_ABI(hello, (hi))
