const char PAGE_MAIN[] PROGMEM = R"=====(

<!DOCTYPE html>
<html lang="en">
<head><!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <title></title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <style>
        .center {
            display: block;
            margin: 20px auto 20px;
            width: 100%
        }
    
        img.center {
            max-width: 800px;
            height: auto;
        }
    
        .row { /** margin auto is only centering the div not the content inside it**/
            margin: 0px auto;
            width: 50%;
            /**border: 3px solid green;**/
            padding: 10px;
            text-align: center; /** this tells the div how to center text **/
        }

        .thinrow {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            padding: 0px;
            text-align: center;
            font-size: 20;
        }
    
        input {
            padding: 5px 5px;
            border: 2px solid gray
        }
    
        input:focus {
            border: 2px solid black
        }
    
        label {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            padding-right: 10px;
        }
    
        p {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            font-size: 12;
            text-align: center;
        }
    
    
    </style>
</head>

<body>
    <img onload="process()" alt="chumbaka" src="https://www.chumbaka.asia/wp-content/uploads/2021/07/chumbaka-with-tag-and-transparent-background.png" class="center">
    <div class="thinrow"><h2>Door log in system</h2></div>
    <div class="row">
        <label for="usernameInput">Username: </label>
        <input id = "usernameInput" onclick = "fixRed()" type="text" name="username">
    </div>
    <div class="row">
        <label for="passwordInput">Password: </label>
        <input id = "passwordInput" onclick = "fixRed()" type="password" name="password">
    </div>
    <div class="row">
        <input type="submit" value="Open the door" onclick="sendToArduino()">
    </div>
    <p id="test1"></p>
    <p id="test2"></p>
    <p>Curious how this works? Learn more <a href = https://github.com/KareemSaysHi/ChumbakaDoorOpener>here!</a></p>
</body>

</html>

<script>

    function wrongInput() {
        document.getElementById("usernameInput").style.border = "2px solid red";
        document.getElementById("passwordInput").style.border = "2px solid red";
        document.getElementById("test1").style.color = "red";
        document.getElementById("test1").innerHTML = "Incorrect username or password";
    }

    function loggedIn() {
        document.getElementById("usernameInput").style.border = "2px solid #00ff00";
        document.getElementById("passwordInput").style.border = "2px solid #00ff00";
        document.getElementById("test1").style.color = "black";
        document.getElementById("test1").innerHTML = "Logged in! Door should be opening";
    }
    
    function fixRed() {
        document.getElementById("usernameInput").style.border = "2px solid black";
        document.getElementById("passwordInput").style.border = "2px solid black";
    }

    function printstuff() {
        console.log(document.getElementById("usernameInput").value);
        console.log(document.getElementById("passwordInput").value);
    }

    function process() {
        console.log("loaded");
        /* this code is important to know, since it runs 
        
        if( xmlHttp.readyState == 0 || xmlHttp.readyState == 4) { //if request not initialized or request just sent
            xmlHttp.open("PUT","xml",true);
            xmlHttp.onreadystatechange=response;
            xmlHttp.send(null);
        }       

        setTimeout("process()",100);  THIS LITERALLY CREATES A RECURSIVE LOOP OF PROCESS, it calls process() in 100ms so this constantly calls for info
        
        Note: I would need a global xmlHttp to take care of constant feedback here but I won't in my website :D */
    
    }

    function sendToArduino() {
        var usernameEntered = document.getElementById("usernameInput").value;
        
        printstuff();
        let xhttp = new XMLHttpRequest();
        
        xhttp.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 200) { //if message recieved and website's doing ok
                // update the web based on reply from  ESP
                console.log(this.responseText);

                if (this.responseText == "found") {
                    document.getElementById("test1").innerHTML ="found";
                    //now call the password function:
                    passwordSend();
                } else {
                    wrongInput();
                    //document.getElementById("test1").innerHTML = "username not found";
                }
            }
        }

        xhttp.open("PUT", "USERNAME_CHECK?VALUE="+usernameEntered, false); //the ? is used as a delimiter on the webserver library side (server.arg)
        xhttp.send();
    }

    function passwordSend() {
        let xhttp = new XMLHttpRequest();
        var passwordEntered = document.getElementById("passwordInput").value; //var variables all have function scope when defined in a function
        xhttp.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 200) { //if message recieved and website's doing ok
                // update the web based on reply from  ESP
                console.log(this.responseText);

                if (this.responseText == "open") {
                    loggedIn();
                    //document.getElementById("test2").innerHTML = "password found, door  opened"
                } else {
                    wrongInput();
                    //document.getElementById("test2").innerHTML = "password not found"
                }
            }
        }

        xhttp.open("PUT", "PASSWORD_CHECK?VALUE="+passwordEntered, false); //the ? is used as a delimiter on the webserver library side (server.arg)
        xhttp.send();
    }

</script>

)=====";