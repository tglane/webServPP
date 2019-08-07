function postText() {
    var oreq = new XMLHttpRequest();
    oreq.addEventListener("load", function () {

        var newDiv = document.createElement("div");
        var content = document.createElement("span");
        content.appendChild(document.createTextNode(this.responseText));
        newDiv.appendChild(content);
        newDiv.appendChild(document.createElement("hr"));

        console.log(newDiv.value);
        var messageWindow = document.getElementById("text_kram");
        messageWindow.appendChild(newDiv);

        /*var parser = new DOMParser();
        console.log(this.responseText);
        let text = parser.parseFromString(this.responseText, "text/html");
        document.getElementById('text_kram').innerHTML = text.getElementById("messages").innerHTML;*/
    });

    oreq.open("post", "/test/ajax");

    oreq.send("new_text=" + document.getElementById("id_text").value);
}
