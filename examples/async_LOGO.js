const snap7 = require("napi-snap7");

const client = new snap7.S7Client();

const time = (ms) => console.log("| Execution time : %d ms", ms);

const res_p = client.SetConnectionParams("192.168.57.12", 0x2000, 0x2100);

const res_c = client.Connect();

// Read out the outputs Q1 - Q8 of a LOGO 0BA8
//            DB number (always 0 for LOGO!s)
//            |    Offset to start
//            |    |   Size to read (bytes)
//            |    |   |
client.DBRead(0, 1064, 1, (err, data) => {
  if (err) return console.log(client.ErrorText(err));
  console.log("Data", data);
  time(client.ExecTime());
  client.Disconnect();
});

/* OR */

// Write a 1 in the bit of the VB address V300.5
const buffer = Buffer.from([0b00100000]);
//             DB number (always 0 for LOGO!s)
//             |   Offset to start
//             |   |   Size to write (bytes)
//             |   |   |     User buffer
//             |   |   |     |
client.DBWrite(0, 300, 1, buffer, (err, data) => {
  if (err) return console.log(client.ErrorText(err));
  console.log("Data", data);
  time(client.ExecTime());
  client.Disconnect();
});