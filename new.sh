cleos create account eosio red1 EOS57HV2rCxkx2fSfAMDLxKKy39kK4ZYfuK15kQiLMdJngj2PZ3S7 EOS57HV2rCxkx2fSfAMDLxKKy39kK4ZYfuK15kQiLMdJngj2PZ3S7
cleos create account eosio red2 EOS57HV2rCxkx2fSfAMDLxKKy39kK4ZYfuK15kQiLMdJngj2PZ3S7 EOS57HV2rCxkx2fSfAMDLxKKy39kK4ZYfuK15kQiLMdJngj2PZ3S7
cleos create account eosio red3 EOS57HV2rCxkx2fSfAMDLxKKy39kK4ZYfuK15kQiLMdJngj2PZ3S7 EOS57HV2rCxkx2fSfAMDLxKKy39kK4ZYfuK15kQiLMdJngj2PZ3S7
cleos create account eosio red4 EOS57HV2rCxkx2fSfAMDLxKKy39kK4ZYfuK15kQiLMdJngj2PZ3S7 EOS57HV2rCxkx2fSfAMDLxKKy39kK4ZYfuK15kQiLMdJngj2PZ3S7
cleos create account eosio red5 EOS57HV2rCxkx2fSfAMDLxKKy39kK4ZYfuK15kQiLMdJngj2PZ3S7 EOS57HV2rCxkx2fSfAMDLxKKy39kK4ZYfuK15kQiLMdJngj2PZ3S7
cleos create account eosio eosio.token EOS57HV2rCxkx2fSfAMDLxKKy39kK4ZYfuK15kQiLMdJngj2PZ3S7 EOS57HV2rCxkx2fSfAMDLxKKy39kK4ZYfuK15kQiLMdJngj2PZ3S7
cleos create account eosio exchange EOS57HV2rCxkx2fSfAMDLxKKy39kK4ZYfuK15kQiLMdJngj2PZ3S7 EOS57HV2rCxkx2fSfAMDLxKKy39kK4ZYfuK15kQiLMdJngj2PZ3S7
cleos create account eosio eosio.msig EOS57HV2rCxkx2fSfAMDLxKKy39kK4ZYfuK15kQiLMdJngj2PZ3S7 EOS57HV2rCxkx2fSfAMDLxKKy39kK4ZYfuK15kQiLMdJngj2PZ3S7

cleos set contract eosio /Users/gaojin/Documents/GitHub/eos/build/contracts/eosio.bios -p eosio
cleos set contract eosio.token /Users/gaojin/Documents/GitHub/eos/build/contracts/eosio.token -p eosio.token
cleos set contract eosio.msig /Users/gaojin/Documents/GitHub/eos/build/contracts/eosio.msig -p eosio.msig
cleos set contract exchange /Users/gaojin/Documents/GitHub/eos/build/contracts/exchange -p exchange
cleos push action eosio.token create '[ "eosio", "1000000000.0000 SYS"]' -p eosio.token
cleos push action eosio.token create '[ "eosio", "1000000000.0000 EOS"]' -p eosio.token
cleos push action eosio.token issue '[ "red4", "100000.0000 SYS", "memo" ]' -p eosio
cleos push action eosio.token issue '[ "red4", "100000.0000 EOS", "memo" ]' -p eosio