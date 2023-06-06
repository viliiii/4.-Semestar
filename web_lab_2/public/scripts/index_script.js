const navKategorije = document.querySelector(".navKategorije").querySelectorAll("li");
const ukupni_brojac_div = document.querySelector(".ukupni_brojac");


if(localStorage.getItem("ukupnibrojac") == null){
    localStorage.setItem("ukupnibrojac", "0");
}

let uk_br_opc = localStorage.getItem("ukupnibrojac");
if(uk_br_opc == 0){
    ukupni_brojac_div.style.opacity = 0;
}else{
    ukupni_brojac_div.style.opacity = 1;
    ukupni_brojac_div.innerText = uk_br_opc;
}



for(let i=0; i<navKategorije.length; i++){
    navKategorije[i].addEventListener("click", () => {
        const slikeContainer = document.querySelector(".slikeContainer");
        slikeContainer.innerHTML = "";

        const trKategorija = document.querySelector(".trenutnaKategorija");
        trKategorija.innerHTML = data.categories[i].name;

        for(let j=0; j<data.categories[i].products.length; j++){
            
            const slika_div = document.createElement("div");
            slika_div.className = "slika";

            const slika_prava = document.createElement("div");
            slika_prava.className = "slika_prava";
            slika_prava.style.background = 'url('+data.categories[i].products[j].image+')';

            const kosarica = document.createElement("img");
            kosarica.className = "kos_hover";
            kosarica.src = "./images/kosarica_icon.png";

            const brojac_div = document.createElement("div");
            brojac_div.className = "krug_brojac";
            brojac_div.style.opacity = 0;
            
            if(localStorage.getItem(data.categories[i].products[j].name) != null){
                brojac_div.innerText = localStorage.getItem(data.categories[i].products[j].name);
            }

            kosarica.addEventListener("click", ()=>{
                if(localStorage.getItem(data.categories[i].products[j].name) == null){
                    localStorage.setItem(data.categories[i].products[j].name, 1);
                    brojac_div.innerText = "1";
                }else{
                    let tr_broj = localStorage.getItem(data.categories[i].products[j].name);
                    tr_broj++;
                    localStorage.removeItem(data.categories[i].products[j].name);
                    localStorage.setItem(data.categories[i].products[j].name, tr_broj);
                    brojac_div.innerText = tr_broj;
                }
                brojac_div.style.opacity = 1;

                uk_br_opc = localStorage.getItem("ukupnibrojac");
                uk_br_opc++;
                localStorage.setItem("ukupnibrojac", uk_br_opc)
                ukupni_brojac_div.innerText = uk_br_opc;
                ukupni_brojac_div.style.opacity = 1;
            })

            if(localStorage.getItem(data.categories[i].products[j].name) != null){
                if(localStorage.getItem(data.categories[i].products[j].name) > 0){
                    brojac_div.style.opacity = 1;
                }
            }else{
                brojac_div.style.opacity = 0;
            }
            


            const tekst_div = document.createElement("div");
            tekst_div.innerText = data.categories[i].products[j].name;


            slika_prava.append(kosarica);
            slika_prava.append(brojac_div);
            slika_div.append(slika_prava);
            slika_div.append(tekst_div);

            document.querySelector(".slikeContainer").appendChild(slika_div);
        }

    })
}

const kosarica1 = document.querySelector(".kosarica");

kosarica1.addEventListener("click", () =>{
    window.location = "../web_labos_1/cart.html"; 
})






































/*na svaku kosaricu action listener koji ce na klik na kosaricu 
dodati ta jproizvod u local storage. Local storage je mapa koja ima kljuc
 i vrijednost koji su
oba stringobi imeproizvoda:kolicina.
ako se to jos ne nalazi u local storage moras stvorit a inace dohvatit iz 
i povecat vrijednost za 1.
sve je to najbolje dodavat u velikom foru, dodavat te stvari*/
