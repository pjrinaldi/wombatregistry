extern crate dirs;
use openssl::pkcs12::Pkcs12;
use std::fs;

pub fn get_certificate(host: &str) -> Option<Pkcs12> {
    let mut key_path = dirs::home_dir().unwrap();
    let mut cert_path = dirs::home_dir().unwrap();
    let key_name = format!("{}.key", host);
    let cert_name = format!("{}.crt", host);

    key_path.push(key_name);
    cert_path.push(cert_name);

    let key = match fs::read(key_path.to_str().unwrap()) {
        Ok(file) => file,
        Err(_) => return None,
    };

    let cert = match fs::read(cert_path.to_str().unwrap()) {
        Ok(file) => file,
        Err(_) => return None,
    };

    let rsa_key = openssl::rsa::Rsa::private_key_from_pem(&key).expect("Invalid RSA key");
    let pkey = openssl::pkey::PKey::from_rsa(rsa_key).expect("Invalid PKey");
    let cert = openssl::x509::X509::from_pem(&cert).expect("Invalid certificate");

    let pkcs_cert = Pkcs12::builder()
        .build("", "", &pkey, &cert)
        .expect("Can't build PKCS12");
    Some(pkcs_cert)
}
