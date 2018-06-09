
eosiocpp -o lbsred-packets.wast lbsred-packets.cpp
eosiocpp -g lbsred-packets.abi lbsred-packets.cpp
cleos create account eosio $1 EOS6zzuh8wUHAmEftGNzHLRDCaxtVmTdBKWNCMDb9rF3DhQMB1XuQ EOS6zzuh8wUHAmEftGNzHLRDCaxtVmTdBKWNCMDb9rF3DhQMB1XuQ
cleos set contract $1  ../lbsred-packets -p $1
cleos get table $1 $1 red
cleos push action $1 sent '["red4","10 EOS",10]' -p $1
cleos get table $1 $1 red
cleos push action $1 take '["red4",1]' -p red4

