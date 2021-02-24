{
    let data = Number(prompt("Decimal number"));

    document.getElementById("text").innerHTML = "Decimal    : "   + data              +
                                                "\nHexidecimal: " + data.toString(16) +
                                                "\nOctal      : " + data.toString(8 ) +
                                                "\nBinary     : " + data.toString(2 ) ;
}