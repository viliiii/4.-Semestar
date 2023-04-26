

let ime = document.getElementById("name");
let email = document.getElementById("email");
let feedback = document.getElementById("feedback");

let savebtn = document.getElementById("savebtn");
let submitbtn = document.getElementById("submitbtn");

if(localStorage.getItem("name") != null){
    ime.value = localStorage.getItem("name");
}

if(localStorage.getItem("email") != null){
    email.value = localStorage.getItem("email");
}

if(localStorage.getItem("feedback") != null){
    feedback.value = localStorage.getItem("feedback");
}



savebtn.addEventListener("click", ()=>{
    if(ime.value == ""){
        alert("Unesite ime.");
    }
    localStorage.setItem("name", ime.value);
    localStorage.setItem("email", email.value);
    localStorage.setItem("feedback", feedback.value);
})

submitbtn.addEventListener("click", ()=>{
    localStorage.removeItem("name");
    localStorage.removeItem("email");
    localStorage.removeItem("feedback");

    ime.value="";
    email.value="";
    feedback.value="";
})