const snap7 = require("napi-snap7");

const client = new snap7.S7Client();

const time = (ms) => console.log("| Execution time : %d ms", ms);

const res = client.ConnectTo("192.168.57.12", 0, 1);

// Reading 100 bytes from DB 510 starts at 0
//                Area identifier (0x84 - S7AreaDB)
//                |    DB number if area = S7AreaDB, otherwise ignored
//                |    |   Offset to start
//                |    |   |   Amount of words to read
//                |    |   |   |     Word size (0x02 - Byte (8 bit))
//                |    |   |   |     |
client.ReadArea(0x84, 510, 0, 100, 0x02, (err, data) => {
  if (err) return console.log(client.ErrorText(err));
  console.log("Data", data);
  time(client.ExecTime());
  client.Disconnect();
});

/* OR */

// Write 42 to address 0 of DB 510
const buffer = Buffer.from([42]);
//                 Area identifier (0x84 - S7AreaDB)
//                 |    DB number if area = S7AreaDB, otherwise ignored
//                 |    |   Offset to start
//                 |    |   |  Amount of words to write
//                 |    |   |  |    Word size (0x02 - Byte (8 bit))
//                 |    |   |  |    |      User buffer
//                 |    |   |  |    |      |
client.WriteArea(0x84, 510, 0, 1, 0x02, buffer, (err, data) => {
  if (err) return console.log(client.ErrorText(err));
  console.log("Data", data);
  time(client.ExecTime());
  client.Disconnect();
});