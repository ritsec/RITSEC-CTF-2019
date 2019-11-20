const express = require('express');
const jwt = require('jsonwebtoken');
const fs = require('fs');
const path = require('path'); 
const router = express.Router();

const app = express();
const port = 4000;

const NOT_FLAG = "Congrats on authenticating! Too bad flags aren't for normal users !!"
const FLAG = "flag{goes-right-here}";
const cert = fs.readFileSync("keys/signing.pem");


router.get('/', (req, res) => {
	res.sendFile(path.join(__dirname+'/index.html'));
});

app.use('/', router);

app.use('/api/', (req, res, next) => {
    if (req.headers.authorization) {
        console.log(`${req.connection.remoteAddress} - ${decode(req.headers.authorization)} - ${req.path}`);
        try {
            let auth = jwt.verify(req.headers.authorization, cert);
            res.locals.auth = auth;
            next();
        } catch (err) {
            console.log(err);
            return res.status(403).send("Invalid JWT or bad signature");
        }
    } else {
        return res.status(401).send("Forbidden, missing JWT authorization");
    }
});

app.get('/info', (req, res) => {
    res.sendFile("info/info.html", {root: __dirname });
});


app.get('/api/normal', (req, res) => {
    if (res.locals.auth && res.locals.auth.type == "user") {
        return res.json({
            flag: NOT_FLAG
        });
    } else {
        res.status(403).json({
            reason: "Not a user!"
        });
    }
});

app.get('/api/admin', (req, res) => {
    if (res.locals.auth && res.locals.auth.type == "admin") {
        return res.json({
            flag: FLAG
        });
    } else {
        res.status(403).json({
            reason: "Not an admin!"
        });
    }
});

app.listen(port, () => console.log(`API server running on ${port}`));

function decode(token) {
	let [header, body, sig] = token.split('.');
	let bh = Buffer.from(header, "base64").toString("ascii");
	let bb = Buffer.from(body, "base64").toString("ascii");
	return [bh, bb, sig];
}
