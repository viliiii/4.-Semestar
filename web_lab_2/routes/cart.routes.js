const data = require('../data/mydata.js');
const express = require('express');
const router = express.Router();



router.get('/getAll', function(req, res, next) {
    var cart = {};

    if(req.session.cart){
        cart = req.session.cart;
    }else{
        req.session.cart = {};
    }

    console.log(req.session.cart);


    res.render('cart', {
        title : 'Cart', 
        cart: req.session.cart
        //linkActive: 'cart'
    });
});

router.get('/add/:id', function(req, res, next){
    req.session.cart.find(obj => obj.ime === req.params.id).kolicina++;

    res.render('cart', {
        title : 'Cart', 
        cart: req.session.cart
    });
});

router.get('/remove/:id', function(req, res, next){
    req.session.cart.find(obj => obj.ime === req.params.id).kolicina--;

    if(req.session.cart.find(obj => obj.ime === req.params.id).kolicina == 0){
        req.session.cart = req.session.cart.filter((item, index) => index !== req.session.cart.findIndex(item => item.ime === req.params.id));
        
    }
    
    
    res.render('cart', {
        title : 'Cart', 
        cart: req.session.cart
    });

});



module.exports = router;