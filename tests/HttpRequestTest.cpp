/*
 * File:   HttpRequestTest.cpp
 * Author: vpl
 *
 * Created on 31 déc. 2014, 11:15:14
 */

#include "../include/HttpRequest.h"
#include "HttpRequestTest.h"
#include "Wt/Json/Array"
#include "Wt/Json/Object"
#include "Wt/Json/Value"
#include <Wt/Utils>


CPPUNIT_TEST_SUITE_REGISTRATION(HttpRequestTest);

HttpRequestTest::HttpRequestTest()
{
}

HttpRequestTest::~HttpRequestTest()
{
}

void HttpRequestTest::setUp()
{
}

void HttpRequestTest::tearDown()
{
}

void HttpRequestTest::testHttpRequest()
{
    std::string email("vianney.plotton@echoes-tech.com");
    std::string token("5C60RQIWJtHfsL1LE8oHRWdp9");
    std::string xmlPageName("units");
    
    HttpRequest httpRequest(email, token, xmlPageName);
    
    CPPUNIT_ASSERT_EQUAL(Wt::Utils::urlEncode(email), httpRequest.getLogin());
    CPPUNIT_ASSERT_EQUAL(token, httpRequest.getToken());
    CPPUNIT_ASSERT_EQUAL(xmlPageName, httpRequest.getXmlPageName());
}

void HttpRequestTest::testHttpRequest2()
{
    std::string apiUrl("http://127.0.0.1:8081");
    
    HttpRequest httpRequest;
    httpRequest.setApiUrl(apiUrl);
    CPPUNIT_ASSERT_EQUAL(apiUrl, httpRequest.getApiUrl());
    
}

void HttpRequestTest::testGetApiAddress()
{
    std::string apiUrl("http://127.0.0.1:8081");
    std::string login("vianney.plotton@echoes-tech.com");
    std::string token("5C60RQIWJtHfsL1LE8oHRWdp9");
    std::string xmlPageName("units");
    std::vector<std::string> listParameter;
    HttpRequest httpRequest;
    httpRequest.setApiUrl(apiUrl);
    httpRequest.setLogin(login);
    httpRequest.setToken(token);
    httpRequest.setXmlPageName(xmlPageName);
    
    std::string result = httpRequest.getApiAddress(httpRequest.getXmlPageName(), listParameter);
    std::string compare = httpRequest.getApiUrl()
            + "/"
            + httpRequest.getXmlPageName()
            + "?login="
            + httpRequest.getLogin()
            + "&token="
            + httpRequest.getToken();
    
    CPPUNIT_ASSERT_EQUAL(compare, result);
    listParameter.push_back("test=1");
    compare += "&test=1";
    result = httpRequest.getApiAddress(httpRequest.getXmlPageName(), listParameter);
    CPPUNIT_ASSERT_EQUAL(compare, result);
}

void HttpRequestTest::testGetApiUrl()
{
    std::string apiUrl("http://127.0.0.1:8081");
    HttpRequest httpRequest;
    httpRequest.setApiUrl(apiUrl);
    
    CPPUNIT_ASSERT_EQUAL(apiUrl, httpRequest.getApiUrl());
}

void HttpRequestTest::testGetLogin()
{
    std::string login("vianney.plotton@echoes-tech.com");
    HttpRequest httpRequest;
    httpRequest.setLogin(login);
    
    CPPUNIT_ASSERT_EQUAL(Wt::Utils::urlEncode(login), httpRequest.getLogin());
}

void HttpRequestTest::testGetToken()
{
    std::string token("5C60RQIWJtHfsL1LE8oHRWdp9");
    HttpRequest httpRequest;
    httpRequest.setToken(token);
    
    CPPUNIT_ASSERT_EQUAL(token, httpRequest.getToken());
}

void HttpRequestTest::testGetXmlPageName()
{
    std::string xmlPageName("units");
    HttpRequest httpRequest;
    httpRequest.setXmlPageName(xmlPageName);
    
    CPPUNIT_ASSERT_EQUAL(xmlPageName, httpRequest.getXmlPageName());
}

void HttpRequestTest::functorJsonValue(Wt::Json::Value value)
{
    Wt::Json::Array unitsArray = (Wt::Json::Array)value;
    Wt::Json::Object unitObject = unitsArray.at(0);
    Wt::WString nameValue = unitObject.get("name");
    std::string compare("unit 1 - text");
    
    CPPUNIT_ASSERT_EQUAL(compare, nameValue.toUTF8());
}

void HttpRequestTest::testSendGet()
{
    std::string apiUrl("http://127.0.0.1:8081");
    std::string login("vianney.plotton@echoes-tech.com");
    std::string token("5C60RQIWJtHfsL1LE8oHRWdp9");
    std::string xmlPageName("units");
    
    HttpRequest httpRequest;
    httpRequest.setApiUrl(apiUrl);
    httpRequest.setLogin(login);
    httpRequest.setToken(token);
    httpRequest.setXmlPageName(xmlPageName);
    
    std::vector<std::string> listParameter;
    httpRequest.sendGet(xmlPageName, listParameter, boost::bind(&HttpRequestTest::functorJsonValue, this, _1));
}

void HttpRequestTest::testSetApiUrl()
{
    std::string apiUrl("http://127.0.0.1:8081/");
    HttpRequest httpRequest;
    httpRequest.setApiUrl(apiUrl);
    CPPUNIT_ASSERT_EQUAL(apiUrl, httpRequest.getApiUrl());
}

void HttpRequestTest::testSetLogin()
{
    std::string login("vianney.plotton@echoes-tech.com");
    HttpRequest httpRequest;
    httpRequest.setLogin(login);
    CPPUNIT_ASSERT_EQUAL(Wt::Utils::urlEncode(login), httpRequest.getLogin());
}

void HttpRequestTest::testSetToken()
{
    std::string token("5C60RQIWJtHfsL1LE8oHRWdp9");
    HttpRequest httpRequest;
    httpRequest.setToken(token);
    CPPUNIT_ASSERT_EQUAL(token, httpRequest.getToken());
}

void HttpRequestTest::testSetXmlPageName()
{
    std::string xmlPageName("units");
    HttpRequest httpRequest;
    httpRequest.setXmlPageName(xmlPageName);
    CPPUNIT_ASSERT_EQUAL(xmlPageName, httpRequest.getXmlPageName());
}

