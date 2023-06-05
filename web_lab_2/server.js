const path = require('path');
const express = require('express');
const session = require('express-session');

const homeRouter = require('./routes/home.routes');
const cartRouter = require('./routes/cart.routes');

const app = express();

app.use(express.urlencoded({ extended: true }));   //ili true?
app.use(express.json());



/* app.get('/data/mydata.js', function(req, res) {
    res.type('application/javascript');
    res.sendFile('./data/mydata.js');
  }); */

    
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'ejs');

app.use(express.static(path.join(__dirname + '/public')));

app.use(
    session({
    secret: "anything",
    resave: false,
    saveUninitialized: true,
    })
); 

app.use('/', homeRouter);
app.use('/cart', cartRouter);


app.listen(3000);