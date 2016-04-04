var express = require('express');
var panel = require('./build/Release/panel.node');


var app = express();
var configured = false;

app.get('/:command', function (req, res) {

	console.log(req.params.command);
	configured = true;

if(req.params.command == 'run'){
    panel.run();
    res.send("OK!");

}else if(req.params.command == 'time'){
	if(configured){
		console.log(panel.time(30));
	}
    res.send("OK!");

}else if(req.params.command == 'exit'){
	if(configured){
		panel.exit('0');
		configured = false;
	}
	res.send("OK!");

}else if(req.params.command == 'zero'){
	if(configured){
		console.log(panel.digital(0));
	}
    res.send("OK!");

}else if(req.params.command == 'um'){
	if(configured){
		console.log(panel.digital(1));
	}
    res.send("OK!");

}else if(req.params.command == 'dois'){
	if(configured){
		console.log(panel.digital(2));
	}
    res.send("OK!");

}else if(req.params.command == 'tres'){
	if(configured){
		console.log(panel.digital(3));
	}
    res.send("OK!");

}else{

	if(configured && !isNaN(req.params.command)){
		console.log(panel.update(parseInt(req.params.command)));
	   
	    res.send("ok");
	}else{
	    res.send("NOK!");

	}
}
	

});





var server = app.listen(80, function () {

  var host = server.address().address;
  var port = server.address().port;
  
  console.log('App listening at http://%s:%s', host, port);
  //panel.setup();
  
  //console.log(panel.update('111'));


});

