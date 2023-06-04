const express = require('express');
const router = express.Router();

router.get('/', function(req, res, next) {
    res.render('home', {
        title : 'Home',
        linkActive: 'home'
    });
});

router.get('/:id', function(req, res, next){
    let category;
    let i = 0;
    
    for (let cat of data.categories){
        if (cat.name === id){
            category = cat.name;
            break;
        }
        i = i+1;
    }
    
    res.render('kategorija', {
        title: category, 
        cat_ind: i


    });
});

module.exports = router;