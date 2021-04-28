const io = require("socket.io-client");
const socket = io("http://localhost:3001/");

setInterval(() => {
    socket.emit("key", {
        bpm: Math.floor(getRandomArbitrary(60, 66)),
        humidity: Math.floor(getRandomArbitrary(80, 83)),
        temp: +getRandomArbitrary(33, 36).toFixed(2),
    });
}, 1000);

function getRandomArbitrary(min, max) {
    return Math.random() * (max - min) + min;
}