const data = require('../data/mydata.js');
const express = require('express');
const router = express.Router();

router.get('/', function (req, res, next) {



    res.render('home', {
        title: 'Home',
        linkActive: 'home',
        cart: req.session.cart
    });
});

router.get('/getProducts/:id', function (req, res, next) {
    let category;
    let i = 0;

    for (let cat of data.categories) {
        if (cat.name === req.params.id) {
            category = cat.name;
            break;
        }
        i = i + 1;
    }

    let brojaci = []
    let ind = 0;
    for (var pro of data.categories[i].products) {
        if (req.session.cart) {
            if (req.session.cart.some(obj => obj.ime === pro.name)) {
                brojaci[ind] = req.session.cart.find(obj => obj.ime === pro.name).kolicina;
            }
        } else {
            brojaci[ind] = 0;
        }
        ind++;
    }

    res.render('kategorija', {
        title: category,
        cat_ind: i,
        data: data,
        cart: req.session.cart,
        brojaci


    });
});

router.get('/getProducts/:category/add/:id', function (req, res, next) {


    let category = req.params.category;

    let auto = {
        ime: req.params.id,
        kolicina: 1
    }

    if (!req.session.cart) {
        req.session.cart = {};
        req.session.cart = [auto];

    } else {
        if (req.session.cart.some(obj => obj.ime === req.params.id)) {
            //vec je nutra takav auto
            req.session.cart.find(obj => obj.ime === req.params.id).kolicina++;
            console.log(req.session.cart.find(obj => obj.ime === req.params.id).kolicina);

        } else {
            //nema jos takvog auta
            req.session.cart.push(auto);

        }
    }

    let j = 0;

    for (var cat of data.categories) {

        if (cat.name == req.params.category) {

            category = cat.name;
            break;
        }
        j++;
    }

    let brojaci = []

    let k = 0;
    for (var pro of data.categories[j].products) {
        if (req.session.cart) {
            if (req.session.cart.some(obj => obj.ime === pro.name)) {
                brojaci[k] = req.session.cart.find(obj => obj.ime === pro.name).kolicina;
            } else {
                brojaci[k] = 0;
            }
        }else{
            brojaci[k]=0
        }
        k++;
    }



    req.session.save();


    res.render('kategorija', {
        title: category,
        data: data,
        cat_ind: j,
        brojaci,
        cart: req.session.cart


    });

});

module.exports = router;