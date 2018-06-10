cleos create account eosio red1 EOS6zzuh8wUHAmEftGNzHLRDCaxtVmTdBKWNCMDb9rF3DhQMB1XuQ EOS6zzuh8wUHAmEftGNzHLRDCaxtVmTdBKWNCMDb9rF3DhQMB1XuQ
cleos create account eosio red2 EOS6zzuh8wUHAmEftGNzHLRDCaxtVmTdBKWNCMDb9rF3DhQMB1XuQ EOS6zzuh8wUHAmEftGNzHLRDCaxtVmTdBKWNCMDb9rF3DhQMB1XuQ
cleos create account eosio red3 EOS6zzuh8wUHAmEftGNzHLRDCaxtVmTdBKWNCMDb9rF3DhQMB1XuQ EOS6zzuh8wUHAmEftGNzHLRDCaxtVmTdBKWNCMDb9rF3DhQMB1XuQ
cleos create account eosio red4 EOS6zzuh8wUHAmEftGNzHLRDCaxtVmTdBKWNCMDb9rF3DhQMB1XuQ EOS6zzuh8wUHAmEftGNzHLRDCaxtVmTdBKWNCMDb9rF3DhQMB1XuQ
cleos create account eosio red5 EOS6zzuh8wUHAmEftGNzHLRDCaxtVmTdBKWNCMDb9rF3DhQMB1XuQ EOS6zzuh8wUHAmEftGNzHLRDCaxtVmTdBKWNCMDb9rF3DhQMB1XuQ
cleos create account eosio eosio.token EOS6zzuh8wUHAmEftGNzHLRDCaxtVmTdBKWNCMDb9rF3DhQMB1XuQ EOS6zzuh8wUHAmEftGNzHLRDCaxtVmTdBKWNCMDb9rF3DhQMB1XuQ
cleos create account eosio exchange EOS6zzuh8wUHAmEftGNzHLRDCaxtVmTdBKWNCMDb9rF3DhQMB1XuQ EOS6zzuh8wUHAmEftGNzHLRDCaxtVmTdBKWNCMDb9rF3DhQMB1XuQ
cleos create account eosio eosio.msig EOS6zzuh8wUHAmEftGNzHLRDCaxtVmTdBKWNCMDb9rF3DhQMB1XuQ EOS6zzuh8wUHAmEftGNzHLRDCaxtVmTdBKWNCMDb9rF3DhQMB1XuQ

cleos set contract eosio /Users/gaojin/Documents/GitHub/eos/build/contracts/eosio.bios -p eosio
cleos set contract eosio.token /Users/gaojin/Documents/GitHub/eos/build/contracts/eosio.token -p eosio.token
cleos set contract eosio.msig /Users/gaojin/Documents/GitHub/eos/build/contracts/eosio.msig -p eosio.msig
cleos set contract exchange /Users/gaojin/Documents/GitHub/eos/build/contracts/exchange -p exchange
cleos push action eosio.token create '[ "eosio", "1000000000.0000 SYS"]' -p eosio.token
cleos push action eosio.token create '[ "eosio", "1000000000.0000 EOS"]' -p eosio.token
cleos push action eosio.token issue '[ "red4", "100000.0000 SYS", "memo" ]' -p eosio
cleos push action eosio.token issue '[ "red4", "100000.0000 EOS", "memo" ]' -p eosio