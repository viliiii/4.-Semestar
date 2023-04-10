/* const slikaPrava = document.querySelectorAll(".slika_prava");

      // petlja koja prolazi kroz svaki div element sa klasom "slika_prava"
      for (let i = 0; i < slikaPrava.length; i++) {
        // postavljanje pozadine za svaki div element
        slikaPrava[i].style.backgroundImage =url('data.categories[0].products[i].image');
      } */



const navKategorije = document.querySelector(".navKategorije").querySelectorAll("li");


for(let i=0; i<navKategorije.length; i++){
    navKategorije[i].addEventListener("click", () => {
        const slikeContainer = document.querySelector(".slikeContainer");
        slikeContainer.innerHTML = "";

        const trKategorija = document.querySelector(".trenutnaKategorija");
        console.log(data.categories[i].name);
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

            const tekst_div = document.createElement("div");
            tekst_div.innerText = data.categories[i].products[j].name;


            slika_prava.append(kosarica);
            slika_div.append(slika_prava);
            slika_div.append(tekst_div);

            document.querySelector(".slikeContainer").appendChild(slika_div);
        }

    })
}
localStorage.getItem()
/*na svaku kosaricu action listener koji ce na klik na kosaricu 
dodati ta jproizvod u local storage. Local storage je mapa koja ima kljuc
 i vrijednost koji su
oba stringobi imeproizvoda:kolicina.
ako se to jos ne nalazi u local storage moras stvorit a inace dohvatit iz 
i povecat vrijednost za 1.
sve je to najbolje dodavat u velikom foru, dodavat te stvari*/
