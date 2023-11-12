const snap7 = require('napi-snap7')

const client = new snap7.S7Client()

const time = (ms) => console.log('| Execution time : %d ms', ms)

const res = client.ConnectTo('192.168.20.55', 0, 1)
console.log('ConnectTo', res)
time(client.ExecTime())
console.log('Connected', client.GetConnected())

// Reading 20 words from DB 510 starts at 0
//                                   Area identifier (0x84 - S7AreaDB)
//                                   |    DB number if area = S7AreaDB, otherwise ignored
//                                   |    |   Offset to start
//                                   |    |   |  Amount of words to read
//                                   |    |   |  |     Word size (0x04 - Word (16 bit))
//                                   |    |   |  |     |
const data_r = client.ReadAreaSync(0x84, 510, 0, 20, 0x04)
console.log('Data', data_r)
time(client.ExecTime())

// Write 42 to address 0 of DB 510
const buffer = Buffer.from([42]);
//                                    Area identifier (0x84 - S7AreaDB)
//                                    |    DB number if area = S7AreaDB, otherwise ignored
//                                    |    |   Offset to start
//                                    |    |   |  Amount of words to write
//                                    |    |   |  |    Word size (0x02 - Byte (8 bit))
//                                    |    |   |  |    |      User buffer
//                                    |    |   |  |    |      |
const data_w = client.WriteAreaSync(0x84, 510, 0, 1, 0x02, buffer)
console.log('Data', data_w)
time(client.ExecTime())

client.Disconnect()