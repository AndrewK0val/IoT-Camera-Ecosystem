const express = require('express')
const app = express()
const server = require('http').Server(app)
const io = require('socket.io')
const {v4: uuidV4} = require('uuid')

app.set('view engine', 'ejs')
app.use(express.static('public'))

app.get('/', (req, res) => {
    res.redirect(`/$(roomId)`)
})

app.get('/:room', (req, res) => {
    res.render('room', { roomId: req.params.room })
})

io.on('connection', socket => {
    socket.on('join-room', (roomId, userId) => {
        socket.join(roomId)
        socket.to(roomId).broadcast.emit('user-connected', userId)
    })
})

server.listen(3000)
// do I know what socket.io does? not a clue