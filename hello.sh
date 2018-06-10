
eosiocpp -o lbsred-packets.wast lbsred-packets.cpp
eosiocpp -g lbsred-packets.abi lbsred-packets.cpp
cleos create account eosio $1 EOS57HV2rCxkx2fSfAMDLxKKy39kK4ZYfuK15kQiLMdJngj2PZ3S7 EOS57HV2rCxkx2fSfAMDLxKKy39kK4ZYfuK15kQiLMdJngj2PZ3S7
cleos set contract $1  ../lbsred-packets -p $1
cleos get table $1 $1 red
cleos set account permission red4 active '{"threshold": 1,"keys": [{"key": "EOS57HV2rCxkx2fSfAMDLxKKy39kK4ZYfuK15kQiLMdJngj2PZ3S7","weight": 1}],"accounts":[{"permission":{"actor":"'$1'","permission":"eosio.code"},"weight":1}]}' owner -p red4
cleos set account permission $1 active '{"threshold": 1,"keys": [{"key": "EOS57HV2rCxkx2fSfAMDLxKKy39kK4ZYfuK15kQiLMdJngj2PZ3S7","weight": 1}],"accounts":[{"permission":{"actor":"'$1'","permission":"eosio.code"},"weight":1}]}' owner -p $1
cleos push action $1 sent '["red4","10.0000 EOS",10]' -p red4
cleos get table $1 $1 red

cleos push action $1 take '["red3",1]' -p red3
cleos push action $1 take '["red2",1]' -p red2
cleos push action $1 take '["red1",1]' -p red1

cleos get table $1 $1 red



