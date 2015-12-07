/* 
 * File:   Enums.h
 * Author: tsa
 *
 * Created on 14 septembre 2012, 15:19
 */

#ifndef ENUMS_H
#define	ENUMS_H

#include "enum.hpp"
#include <string.h>

class Enums
{
public:
    Enums();
    Enums(const Enums& orig);
    virtual ~Enums();

    enum EMedia
    {
        email = 1,
        sms = 2,
        mobileapp = 3
    };

    enum EAction
    {
        login = 1,
        registration = 2,
        logout = 3,
        display = 4,
        download = 5,
        add = 6,
        del = 7
    };

    enum EInformationUnitType
    {
        text = 1,
        number = 2,
        boolean = 3,
        custom = 4
    };

    enum EOption
    {
        quotaAsset = 1,
        quotaSms = 2
    };

    enum EMenuRoot
    {
        main
    };

    enum EReturnCode
    {
        OK = 200,
        CREATED = 201,
        ACCEPTED = 202,
        NO_CONTENT = 204,
        RESET_CONTENT = 205,
        PARTIAL_CONTENT = 206,

        MULTIPLE_CHOICES = 300,
        MOVED_PERMANENTLY = 301,
        FOUND = 302,
        SEE_OTHER = 303,
        NOT_MODIFIED = 304,
        USE_PROXY = 305,
        TEMPORARY_REDIRECT = 307,
        TOO_MANY_REDIRECT = 310,

        BAD_REQUEST = 400,
        UNAUTHORIZED = 401,
        PAYMENT_REQUIRED = 402,
        FORBIDDEN = 403,
        NOT_FOUND = 404,
        METHOD_NOT_ALLOWED = 405,
        NOT_ACCEPTABLE = 406,
        PROXY_AUTHENTICATION_REQUIRED = 407,
        REQUEST_TIME_OUT = 408,
        CONFLICT = 409,
        GONE = 410,
        LENGTH_REQUIRED = 411,
        PRECONDITION_FAILED = 412,
        REQUEST_ENTITY_TOO_LARGE = 413,
        REQUEST_URI_TOO_LONG = 414,
        UNSUPPORTED_MEDIA_TYPE = 415,
        REQUESTED_RANGE_UNSATISFIABLE = 416,
        EXPECTATION_FAILED = 417,
        I_AM_A_TEAPOT = 418,
        LOCKED = 423,
        UPGRADE_REQUIRED = 426,

        INTERNAL_SERVER_ERROR = 500,
        NOT_IMPLEMENTED = 501,
        BAD_GATEWAY_OR_PROXY_ERROR = 502,
        SERVICE_UNAVAILABLE = 503,
        GATEWAY_TIME_OUT = 504,
        HTTP_VERSION_NOT_SUPPORTED = 505,
        BANDWIDTH_LIMIT_EXCEDED = 509
    };

    BOOST_ENUM_VALUES(EPageType, const char*,
                        (MESSAGES) ("messages")
                        //(WELCOME) ("welcome")
                        (DASHBOARD) ("dashboard")
                        (PROBES) ("probes")                  
                        (ASSET) ("assets")
                        (RECIPIENTS) ("recipients")
                        (INFORMATIONS) ("informations")
                        (ALERTS) ("alerts")
                        (PLUGIN) ("plugins")
                        (ALIASES) ("aliases")
                        (ROLES) ("roles")
                        (OPTIONS) ("options")
                        (UNITS) ("units")
                        (HELP) ("help")
                      )
    
    BOOST_ENUM_VALUES(EMessageStatus, const char*,
                        (UNCREATED) ("uncreated")
                        (CREATED) ("created")
                        (SENDFAILED) ("send failed")
                        (SENDREFUSED) ("send refused")
                        (LINKING) ("linking")
                        (LINKLOST) ("link lost")
                        (SENDED) ("sended")
                        (ACKFAILED) ("ack failed")
                        (RECEIVED) ("received")
                        (ANSWERED) ("answered")
                      )
    BOOST_ENUM_VALUES(EAlertStatus, const char*,
                        (PENDING) ("pending")
                        (SUPPORTED) ("supported")
                        (FORWARDING) ("forwarding")
                        (BACKTONORMAL) ("back to normal")                        
                      )
private:

};

#endif	/* ENUMS_H */

