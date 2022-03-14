const snap7 = require('napi-snap7')

const client = new snap7.S7Client()

const time = (ms) => console.log('| Execution time : %d ms', ms)

const res = client.ConnectTo('192.168.20.55', 0, 1)
console.log('ConnectTo', res)
time(client.ExecTime())
console.log('Connected', client.GetConnected())

const data = client.ReadAreaSync(0x84, 510, 0, 20, 0x04)
console.log('Data', data)
time(client.ExecTime())

client.Disconnect()