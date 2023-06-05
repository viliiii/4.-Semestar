const data = require('../data/mydata.js');
const express = require('express');
const router = express.Router();

router.get('/', function(req, res) {
    res.render('cart', {
        title : 'Cart'
        //linkActive: 'cart'
    });
});



module.exports = router;