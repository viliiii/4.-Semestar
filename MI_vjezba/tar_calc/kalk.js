
let left;
let right;
let bool_left;
let bool_right;
let operacije;
let last_operand;
let result;
postavi();

document.querySelectorAll(".number").forEach(gumb => {
    gumb.addEventListener("click", () => {

        let display = document.getElementById("display").innerText;
        display += gumb.innerText;
        document.getElementById("display").innerText = display;

        if (bool_left) {
            left += gumb.innerText;
            result += gumb.innerText;
        } else {
            right += gumb.innerText;
        }
        
    })
})

document.getElementById("ac").addEventListener("click", () => {
    document.getElementById("display").innerText = "";
    postavi();
})

document.querySelectorAll(".operacija").forEach(operacija => {
    operacija.addEventListener("click", () => {
        let display = document.getElementById("display").innerText;
        display += operacija.innerText;
        document.getElementById("display").innerText = display;

        if (operacija.innerText == "+" || operacija.innerText == "-") {

            if (bool_right) {
                let rez;
                if (last_operand == "*") {
                    rez = left * right;
                } else if (last_operand == "/"){
                    rez = left / right;
                }

                if (operacije != "") {
                    result = result.slice(0, result.lastIndexOf(operacije[operacije.length -1] + 1));
                    result += rez;
                } else {
                    result = rez;
                }

                
                bool_left = true;
                bool_right = false;

            }

            left = "";
            operacije += operacija.innerText;
            result += operacija.innerText;

        } else {

            if (bool_right) {

                let rez;
                if (last_operand == "*") {
                    rez = left * right;
                } else if (last_operand == "/"){
                    rez = left / right;
                }

                if (operacije != "") {
                    result = result.slice(0, result.lastIndexOf(operacije[operacije.length -1] + 1));
                    result += rez;
                } else {
                    result = rez;
                }

                left = rez;
            }
            right = "";
            last_operand = operacija.innerText;
            bool_left = false;
            bool_right = true;
        }

    })
})

document.getElementById("equal").addEventListener("click", ()=>{
    let izraz = document.getElementById("display").innerText;

    if (bool_right) {
        let rez;
        if (last_operand == "*"){
            rez = left * right;
        } else {
            rez = left / right;
        }

        if (operacije != "") {
            result = result.slice(0, result.lastIndexOf(operacije[operacije.length-1]) + 1);
            result += rez;
        } else {
            result = rez;
        }
       
    }

    for (let i = 0; i < operacije.length; i++) {
        let left = result.substring(0, result.indexOf(operacije[i]));
        let right = "";
        let index1;
        let index2;
        if (i + 1 < operacije.length) {
            index1 = result.indexOf(operacije[i]) +1;
            index2 = (result.substring(result.indexOf(operacije[i]) + 1)).indexOf(operacije[i + 1]) + left.length + 1;
            right = result.substring(index1, index2);
        } else {
            right = result.substring(result.indexOf(operacije[i]) + 1);
        }
        
        
        if (operacije[i] == "+"){
            let rez = parseFloat(left) + parseFloat(right);

            if (i + 1 < operacije.length) {
                result = result.replace(result.substring(0, index2), rez);
            } else {
                result = result.replace(result.substring(0), rez);
               
            }
            
        } else {
            let rez = parseFloat(left) - parseFloat(right);

            if (i + 1 < operacije.length) {
                result = result.replace(result.substring(0, index2), rez);
            } else {
                
                result = result.replace(result.substring(0), rez);
                
            }
        }
    }
    
    document.getElementById("display").innerText = result;
    console.log(result);
})

/*document.getElementById("point").addEventListener("click", () => {
    let display = document.getElementById("display").innerText;
    display += document.getElementById("point").innerText;
    document.getElementById("display").innerText = display;
})*/


function postavi() {
    left = "";
    right = "";
    bool_left = true;
    bool_right = false;
    operacije = "";
    last_operand = "";
    result = "";

}