import fs from "fs";
import crypto from 'crypto';
import cp from 'child_process';

let key  = fs.readFileSync('pub.pem', 'utf8');
let sig = fs.readFileSync('sig.txt', 'utf8');
let message = fs.readFileSync('message.txt', 'utf8');
let hash = cp.exec('openssl sha1 message.txt', (err, stdout, stderr) => {
    if(err) {
        console.log(err);
        hash = "SHA1(message.txt)= 30781fa28aacb1785eb363ab9f61722de60df337\n"
    } //i tilfelle du ikke har openssl elns
    else hash = stdout;
    let sigbuf = Buffer.from(sig, 'hex');

    let dec_sig = crypto.publicDecrypt(key, sigbuf);

    console.log("dekryptert sig: ", dec_sig);
    console.log("sha1 av hash: "+hash);
    console.log("stemmer overens ja?")
});

