class Student{
    constructor(ime, prezime) {
        this.ime = ime;
        this.prezime = prezime;
        
        this.jmbag = '0036';

        for(let i=0; i<6; i++){
            this.jmbag = this.jmbag + Math.floor(Math.random() * 10);;
        }
    }
    
    toString(){
        let zaReturn = `${this.ime} ${this.prezime}, ${this.jmbag}`;

        return zaReturn;
    }
}

let x = new Student("Pero", "Perić");

console.log(x.toString());
