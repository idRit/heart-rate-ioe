const io = require("socket.io-client");
const socket = io("http://localhost:3001/");

const q = 'hetalq';

function bail(err) {
    console.error(err);
    process.exit(1);
}

// Consumer
function consumer(conn) {
    var ok = conn.createChannel(on_open);
    function on_open(err, ch) {
        if (err != null) bail(err);
        ch.assertQueue(q);
        ch.consume(q, function (msg) {
            if (msg !== null) {
                console.log(msg.content.toString());
                socket.emit("key", JSON.parse(msg.conent.toString()));
                ch.ack(msg);
            }
        });
    }
}

require('amqplib/callback_api')
    .connect('amqps://ylhnwvqf:U0gpsudyXQBu8LiwAIOc7um4Av9ajPBd@lion.rmq.cloudamqp.com/ylhnwvqf', function (err, conn) {
        if (err != null) bail(err);
        consumer(conn);
    });