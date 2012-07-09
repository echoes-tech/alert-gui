#include "Session.h"

using namespace Wt;

namespace
{

class MyOAuth : public std::vector<const Auth::OAuthService *>
{
public:
    ~MyOAuth()
    {
        for (unsigned i = 0; i < size(); ++i)
            delete (*this)[i];
    }
};

Auth::AuthService myAuthService;
Auth::PasswordService myPasswordService(myAuthService);
MyOAuth myOAuthServices;

}

void Session::configureAuth()
{
    myAuthService.setAuthTokensEnabled(true, "logincookie");
    myAuthService.setEmailVerificationEnabled(true);

    Auth::PasswordVerifier *verifier = new Auth::PasswordVerifier();
    verifier->addHashFunction(new Auth::BCryptHashFunction(7));
    myPasswordService.setVerifier(verifier);
    myPasswordService.setAttemptThrottlingEnabled(true);
    myPasswordService.setStrengthValidator(new Auth::PasswordStrengthValidator());

    if (Auth::GoogleService::configured())
        myOAuthServices.push_back(new Auth::GoogleService(myAuthService));
}

Session::Session()
{
    
}

Session::Session(Wt::Dbo::backend::Postgres *pgBackend)
{

    setConnection(*pgBackend);

    mapClass<User>("T_USER_USR");
    mapClass<AuthInfo>("auth_info");
    mapClass<AuthInfo::AuthIdentityType>("auth_identity");
    mapClass<AuthInfo::AuthTokenType>("auth_token");
//    mapClass<AuthInfo>("T_AUTH_INFO_AIN");
//    mapClass<AuthInfo::AuthIdentityType>("T_AUTH_IDENTITY_AID");
//    mapClass<AuthInfo::AuthTokenType>("T_AUTH_TOKEN_ATO");

    try
    {
        createTables();
        std::cerr << "Created database." << std::endl;
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << "createTables problem : Session.cpp";
    }

    users_ = new UserDatabase(*this);
}

Session::~Session()
{
    delete users_;
}

Auth::AbstractUserDatabase& Session::users()
{
    return *users_;
}

Wt::Dbo::ptr<User> Session::user() const
{
    if (login_.loggedIn())
    {
        Wt::Dbo::ptr<AuthInfo> authInfo = users_->find(login_.user());
        return authInfo->user();
    }
    else
        return Wt::Dbo::ptr<User>();
}

const Auth::AuthService& Session::auth()
{
    return myAuthService;
}

const Auth::PasswordService& Session::passwordAuth()
{
    return myPasswordService;
}

const std::vector<const Auth::OAuthService *>& Session::oAuth()
{
    return myOAuthServices;
}
