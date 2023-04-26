async function LoadData(fileName){
    let promise = await fetch(fileName);
    if (!promise.ok) { throw new Error("Cannot load json file"); } 
    else { return await promise.json();}     
}

let loadanjeC = LoadData("categories.json").then(
    function(cats){
        LoadData("products.json").then(
            function(prods){
                let  lista = Array(100).fill(0);
                let max = 0;
                let indMax = 0;
                prods.forEach(element => {
                    lista[element.CategoryId] =  lista[element.CategoryId] + element.NumProd;
                });

                for(let i=0; i<100; i++){
                    if(lista[i] > max){
                        max = lista[i];
                        indMax = i;
                    }
                }

                let string = `Category with most products: ${cats[indMax - 1].CategoryName} - ${max}`;
                console.log(string);
            }
        )
    }
).catch( err => {
    console.log("An error has occured and program stopped working");
})


