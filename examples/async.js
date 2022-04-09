const snap7 = require("napi-snap7");

const client = new snap7.S7Client();

const time = (ms) => console.log("| Execution time : %d ms", ms);

const res = client.ConnectTo("192.168.57.12", 0, 1);

client.ReadArea(0x84, 510, 0, 100, 0x02, (err, data) => {
  if (err) return console.log(client.ErrorText(err));
  console.log("Data", data);
  time(client.ExecTime());
  client.Disconnect();
});
