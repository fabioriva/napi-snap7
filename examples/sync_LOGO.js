const snap7 = require('napi-snap7')

const client = new snap7.S7Client()

const time = (ms) => console.log('| Execution time : %d ms', ms)

const res_p = client.SetConnectionParams("192.168.57.12", 0x2000, 0x2100);

const res_c = client.Connect();
console.log('ConnectTo', res_c)
time(client.ExecTime())
console.log('Connected', client.GetConnected())

// Read out the outputs Q1 - Q8 of a LOGO 0BA8
//                               DB number (always 0 for LOGO!s)
//                               |    Offset to start
//                               |    |   Size to read (bytes)
//                               |    |   |
const data_r = client.DBReadSync(0, 1064, 1)
console.log('Data', data_r)
time(client.ExecTime())

// Write a 1 in the bit of the VB address V300.5
const buffer = Buffer.from([0b00100000]);
//                                DB number (always 0 for LOGO!s)
//                                |   Offset to start
//                                |   |   Size to write (bytes)
//                                |   |   |     User buffer
//                                |   |   |     |
const data_w = client.DBWriteSync(0, 300, 1, buffer)

client.Disconnect()