var panel = require('./build/Release/panel.node');
var express = require('express');
var app = express();

var server = require('http').createServer(app);
var io = require('socket.io')(server);
var port = process.env.PORT || 80;


server.listen(port, function () {
    console.log('Server listening at port %d', port);
});

var configured = false, authenticated = false;

io.on('connection', function (socket) {
    var password = "";
    var posicao = 1;

    socket.on('new connection', function (data) {
        // fazer acesso ao rlms para autenticar
        //password = data.pass;
        console.log('new connection' + data);
        posicao = 1;
        authenticated = true;
        var res = {};
        if (panel.run()) {
            panel.update(posicao);
            panel.digital(posicao);
            res.pos = posicao;
            socket.emit('initial', res);
            panel.exit();
        }
    });

    socket.on('new message', function (data) {
        var res = {};
        console.log('new message :' + data);
        if (authenticated) {
            if (panel.run()) {
                var newpos;
                if (data.key == "right") {
                    newpos = ((posicao + 1) % 5 == 0) ? 1 : posicao + 1;
                } else {
                    newpos = ((posicao - 1) % 5 == 0) ? 4 : posicao - 1;
                }
                console.log("pos: " + newpos);
                panel.update(newpos);
                res.pos = newpos - 1;
                socket.emit('initial', res);
                posicao = newpos;
                panel.exit();
            }
        }
    });

    socket.on('disconnect', function () {
        if (panel.run()) {
            panel.update(1);
            panel.digital(0);
            panel.exit();
        }
        configured = authenticated = false;
        console.log('disconnected');
    });


});

app.get('/:command', function (req, res) {
    console.log(req.params.command);
    if (panel.run()) {
        var pos = 1;
        var newpos;
        if (req.params.command == 'right') {
            newpos = ((pos + 1) % 5 == 0) ? 1 : pos + 1;
        } else if (req.params.command == 'left') {
            newpos = ((pos - 1) % 5 == 0) ? 4 : pos - 1;
        }
        console.log("pos: " + newpos);
        panel.digital(newpos);
        res.send(newpos);
        panel.exit();
    }

});
