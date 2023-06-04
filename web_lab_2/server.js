const path = require('path');
const express = require('express');

const homeRouter = require('./routes/home.routes');
const cartRouter = require('./routes/cart.routes');

const app = express();

app.use(express.urlencoded({ extended: false }));   //ili true?
app.use(express.json());

/* app.use(
    session({
    secret: "anything",
    resave: false,
    saveUninitialized: true,
    })
);  */

    
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'ejs');

app.use(express.static(path.join(__dirname + '/public')));

app.use('/', homeRouter);
//app.use('/cart', cartRouter);


app.listen(3000);