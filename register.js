function checkForm() {
    let areFormErrors = false;
    let errorMessages = "<ul>";
    const fullNameElement = document.getElementById("fullName");
    if (fullNameElement.value.length === 0) {
        errorMessages += "<li>Missing full name.</li>";
        fullNameElement.classList.add("error");
        areFormErrors = true;
    } else {
        fullNameElement.classList.remove("error");
        areFormErrors = false;
    }
    errorMessages += "</ul>";
    if (areFormErrors){
        document.getElementById("formErrors").innerHTML = errorMessages;
        document.getElementById("formErrors").classList.remove("hide");
    } else {
        document.getElementById("formErrors").classList.add("hide");
    }
    // check email syntax
    // check password meets requirements 
 }
 
 document.getElementById("submit").addEventListener("click", function(event) {
    checkForm();
 
    // Prevent default form action. DO NOT REMOVE THIS LINE
    event.preventDefault();
 });