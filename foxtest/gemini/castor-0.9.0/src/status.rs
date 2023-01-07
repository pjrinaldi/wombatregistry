extern crate regex;
use regex::Regex;

use std::str::FromStr;

#[derive(Debug)]
pub enum Status {
    Input(String),
    Success(String),
    SuccessEndOfClientCertificateSession(String),
    RedirectTemporary(String),
    RedirectPermanent(String),
    TemporaryFailure(String),
    ServerUnavailable(String),
    CGIError(String),
    ProxyError(String),
    SlowDown(String),
    PermanentFailure(String),
    NotFound(String),
    Gone(String),
    ProxyRequestRefused(String),
    BadRequest(String),
    ClientCertificateRequired(String),
    TransientCertificateRequired(String),
    AuthorisedCertificatedRequired(String),
    CertificateNotAccepted(String),
    FutureCertificateRejected(String),
    ExpiredCertificateRejected(String),
    Unknown(String),
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub struct ParseError;

const STATUS_REGEX: &str = r"^(\d{1,3})[ \t](.*)\r\n$";

impl FromStr for Status {
    type Err = ParseError;

    // Parses a &str into an instance of 'Header'
    fn from_str(line: &str) -> Result<Self, Self::Err> {
        let status_regexp = Regex::new(STATUS_REGEX).unwrap();

        match status_regexp.captures(&line) {
            Some(caps) => {
                let code_str = caps.get(1).map_or("", |m| m.as_str());
                let meta_str = caps.get(2).map_or("", |m| m.as_str());

                let code: i16 = code_str.parse().expect("2");
                let meta = meta_str.to_string();

                Ok(make_status(code, meta))
            }
            None => Err(ParseError),
        }
    }
}

fn make_status(code: i16, meta: String) -> Status {
    match code {
        10 => Status::Input(meta),
        20 => Status::Success(meta),
        21 => Status::SuccessEndOfClientCertificateSession(meta),
        30 => Status::RedirectTemporary(meta),
        31 => Status::RedirectPermanent(meta),
        40 => Status::TemporaryFailure(meta),
        41 => Status::ServerUnavailable(meta),
        42 => Status::CGIError(meta),
        43 => Status::ProxyError(meta),
        44 => Status::SlowDown(meta),
        50 => Status::PermanentFailure(meta),
        51 => Status::NotFound(meta),
        52 => Status::Gone(meta),
        53 => Status::ProxyRequestRefused(meta),
        59 => Status::BadRequest(meta),
        60 => Status::ClientCertificateRequired(meta),
        61 => Status::TransientCertificateRequired(meta),
        62 => Status::AuthorisedCertificatedRequired(meta),
        63 => Status::CertificateNotAccepted(meta),
        64 => Status::FutureCertificateRejected(meta),
        65 => Status::ExpiredCertificateRejected(meta),
        _ => Status::Unknown(meta),
    }
}
