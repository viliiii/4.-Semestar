const proizvodi = document.querySelector(".proizvodi");

if(localStorage.getItem("ukupnibrojac") == null){
    localStorage.setItem("ukupnibrojac", "0");
}

for(let i=0; i<data.categories.length; i++){
    for(let j=0; j<data.categories[i].products.length; j++){
        if(localStorage.getItem(data.categories[i].products[j].name) != null){
            const naziv_kolicina_div = document.createElement("div");
    naziv_kolicina_div.className = "naziv_kolicina";

    const naziv_div = document.createElement("div");
    naziv_div.className = "naziv";
    naziv_div.innerText = data.categories[i].products[j].name;

    const minus_div = document.createElement("div");
    minus_div.className = "minus";
    minus_div.innerText = "-";
    minus_div.addEventListener("click", ()=>{
        let min_tr_br = localStorage.getItem(data.categories[i].products[j].name);
        if(min_tr_br == 1){
            localStorage.removeItem(data.categories[i].products[j].name);
            let uk_br = localStorage.getItem("ukupnibrojac");
            uk_br--;
            localStorage.setItem("ukupnibrojac", uk_br);
            location.reload();
            
        }else{
            min_tr_br--;
            localStorage.removeItem(data.categories[i].products[j].name);
            localStorage.setItem(data.categories[i].products[j].name, min_tr_br);
            kolicina_div.innerText = min_tr_br;

            let uk_br = localStorage.getItem("ukupnibrojac");
            uk_br--;
            localStorage.setItem("ukupnibrojac", uk_br);
    
        }
    })

    const kolicina_div = document.createElement("div");
    kolicina_div.className = "kolicina";
    kolicina_div.innerText = localStorage.getItem(data.categories[i].products[j].name);

    const plus_div = document.createElement("div");
    plus_div.className = "plus";
    plus_div.innerText = "+";
    plus_div.addEventListener("click", ()=>{
        let tr_br = localStorage.getItem(data.categories[i].products[j].name);
        tr_br++;
        localStorage.removeItem(data.categories[i].products[j].name);
        localStorage.setItem(data.categories[i].products[j].name, tr_br);
        kolicina_div.innerText = tr_br;

        let uk_br = localStorage.getItem("ukupnibrojac");
        uk_br++;
        localStorage.setItem("ukupnibrojac", uk_br);
    })


            naziv_kolicina_div.append(naziv_div);
            naziv_kolicina_div.append(minus_div);
            naziv_kolicina_div.append(kolicina_div);
            naziv_kolicina_div.append(plus_div);

            proizvodi.append(naziv_kolicina_div);
        }
        
    }



}

