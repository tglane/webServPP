function postText() {
    var oreq = new XMLHttpRequest();
    oreq.addEventListener("load", function () {
        var parser = new DOMParser();
        let text = parser.parseFromString(this.responseText, "text/html");
        document.getElementById('text_kram').innerHTML = text.getElementById("messages").innerHTML;
    });

    oreq.open("post", "/test/ajax");

    if(document.getElementById("text_kram") != null) {
        oreq.send("new_text=" + document.getElementById("id_text").value + "&old_text=" + document.getElementById('text_kram').innerHTML);
    } else {
        oreq.send("new_text=" + document.getElementById("id_text").value + "&old_text");
    }
}
