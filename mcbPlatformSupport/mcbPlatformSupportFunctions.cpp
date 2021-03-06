//
//  mcbPlatformSupportFunctions.cpp
//  mcb
//
//  Created by Radif Sharafullin on 6/8/13.
//
//

#include "mcbPlatformSupportFunctions.h"
#include "mcbPlatformSupportConstants.h"
#include "mcbPlatformSupport.h"
#include "mcbNumber.h"

using namespace cocos2d;
namespace mcb{namespace PlatformSupport{namespace Functions{
    
    
    
    cocos2d::CCPoint relativeCoordinatesFromAbsolute(cocos2d::CCPoint absoluteCoordinates){
        cocos2d::CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
        cocos2d::CCRect visibleRect(mcb::PlatformSupport::getVisibleScreenRect());
        absoluteCoordinates.x=absoluteCoordinates.x/screenSize.width*visibleRect.size.width+visibleRect.origin.x;
        absoluteCoordinates.y=absoluteCoordinates.y/screenSize.height*visibleRect.size.height+visibleRect.origin.y;
        return absoluteCoordinates;
    }
    cocos2d::CCPoint relativeScaleFromAbsolute(cocos2d::CCPoint absoluteScale){
        cocos2d::CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
        cocos2d::CCRect visibleRect(mcb::PlatformSupport::getVisibleScreenRect());
        absoluteScale.x=absoluteScale.x/screenSize.width*visibleRect.size.width;
        absoluteScale.y=absoluteScale.y/screenSize.height*visibleRect.size.height;
        return absoluteScale;
    }
    float normalizeAngle(float angle){
        if (angle > 0)
            angle = fmodf(angle, 360.0f);
        else
            angle = fmodf(angle, -360.0f);
        return angle;
    }
    
    
    //parser
    
    cocos2d::CCPoint ccpFromString(const std::string & ccpString)
    {
        cocos2d::CCPoint p;
        
        cocos2d::CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
        
        size_t loc = ccpString.find(Constants::kRelativePointOnScreen);
        size_t loc1 = ccpString.find(Constants::kRelativePositionFromAbsoluteString);
        size_t loc2 = ccpString.find(Constants::kRelativePositionFromCenter);
        
        if (loc != ccpString.npos) {
            std::string point = ccpString.substr(loc+1);
            p = cocos2d::CCPoint(CCPointFromString(point.c_str()));
            cocos2d::CCRect visibleRect(mcb::PlatformSupport::getVisibleScreenRect());
            p.x=p.x*visibleRect.size.width+visibleRect.origin.x;
            p.y=p.y*visibleRect.size.height+visibleRect.origin.y;
        }else if (loc1 != ccpString.npos) {
            std::string point(ccpString.substr(loc1+1));
            p = relativeCoordinatesFromAbsolute(cocos2d::CCPoint(CCPointFromString(point.c_str())));
        }else if (loc2 != ccpString.npos) {
            std::string point(ccpString.substr(loc1+1));
            p=cocos2d::ccpAdd(CCPointFromString(point.c_str()), mcb::PlatformSupport::getScreenCenter());
        }else if (ccpString==Constants::kCenterPositionString)
            p = cocos2d::CCPoint(screenSize.width/2, screenSize.height/2);
        else if (ccpString==Constants::kTopLeftPositionString)
            p = cocos2d::CCPoint(0, screenSize.height);
        else if (ccpString==Constants::kTopRightPositionString)
            p = cocos2d::CCPoint(screenSize.width, screenSize.height);
        else if (ccpString==Constants::kBottomLeftPositionString)
            p = cocos2d::CCPointZero;
        else if (ccpString==Constants::kBottomRightPositionString)
            p = cocos2d::CCPoint(screenSize.width, 0);
        else
            p = cocos2d::CCPoint(CCPointFromString(ccpString.c_str()));
        
        return p;
    }
    
    std::string stringForObjectKey(cocos2d::CCDictionary *dict, const std::string & key, const std::string & defaultVal){
        CCString * s(dynamic_cast<CCString *>(dict->objectForKey(key.c_str())));
        if (!s)
            return defaultVal;
        return s->m_sString;
    }
    
    bool boolForObjectKey(cocos2d::CCDictionary *dict, const std::string & key, bool defaultVal){
        bool retVal(defaultVal);
        CCObject * obj(dict->objectForKey(key.c_str()));
        PlatformSupport::Number * n(dynamic_cast<PlatformSupport::Number *>(obj));
        if (n) {
            retVal=n->value;
        }else{
            CCString * pString(dynamic_cast<CCString *>(obj));
            if (pString)
                retVal=pString->boolValue();
        }
        return retVal;
    }
    
    float floatForObjectKey(cocos2d::CCDictionary *dict, const std::string & key, float defaultVal){
        float retVal(defaultVal);
        CCObject * obj(dict->objectForKey(key.c_str()));
        PlatformSupport::Number * n(dynamic_cast<PlatformSupport::Number *>(obj));
        if (n) {
            retVal=n->value;
        }else{
            CCString * pString(dynamic_cast<CCString *>(obj));
            if (pString)
                retVal=pString->intValue();
        }
        return retVal;
    }
    
    cocos2d::CCSize sizeForObjectKey(cocos2d::CCDictionary *dict, const std::string & key, cocos2d::CCSize defaultVal){
        CCPoint sizePoint(pointForObjectKey(dict, key, {defaultVal.width, defaultVal.height}));
        return {sizePoint.x, sizePoint.y};
    }
    cocos2d::CCPoint pointForObjectKey(cocos2d::CCDictionary *dict, const std::string & key, cocos2d::CCPoint defaultVal){
        CCPoint retVal(defaultVal);
        CCObject * obj(dict->objectForKey(key.c_str()));
        CCString * pString(dynamic_cast<CCString *>(obj));
        if (pString)
            retVal=PlatformSupport::Functions::ccpFromString(pString->m_sString);
        return retVal;
    }
    
    cocos2d::ccColor3B colorForObjectKey(cocos2d::CCDictionary *dict, const std::string & key, cocos2d::ccColor3B defaultVal){
        ccColor3B retVal(defaultVal);
        CCObject * obj(dict->objectForKey(key.c_str()));
        CCArray * colors(dynamic_cast<CCArray *>(obj));
        if (colors && colors->count()>2) {
            retVal.r=((Number *)colors->objectAtIndex(0))->value;
            retVal.g=((Number *)colors->objectAtIndex(1))->value;
            retVal.b=((Number *)colors->objectAtIndex(2))->value;
        }
        return retVal;
    }
    
    cocos2d::ccColor3B webColorForObjectKey(cocos2d::CCDictionary *dict, const std::string & key, cocos2d::ccColor3B defaultVal){
        CCString * colorS(dynamic_cast<CCString *>(dict->objectForKey(key.c_str())));
        if (colorS)
            return parseWebRGB(colorS->m_sString);
        return defaultVal;
    }
    
    cocos2d::CCTextAlignment textAlignmentForObjectKey(cocos2d::CCDictionary *dict, const std::string & key, cocos2d::CCTextAlignment defaultVal){
        CCTextAlignment retVal{defaultVal};
        CCObject * obj(dict->objectForKey(key.c_str()));
        CCString * str(dynamic_cast<CCString *>(obj));
        if (str) {
            if (str->m_sString=="left")
                retVal=kCCTextAlignmentLeft;
            else if (str->m_sString=="right")
                retVal=kCCTextAlignmentRight;
        }
        return retVal;
    }
    
    void setNodeProperties(cocos2d::CCNode * node, CCDictionary * data){
        
        CCPoint pos(Functions::pointForObjectKey(data, "position"));
        float rotation(Functions::floatForObjectKey(data, "rotation"));
        
        //scale
        bool useScaleXY(false);
        float scaleX(1.f);
        float scaleY(1.f);
        
        float scale(1.f);
        Number * num((Number *)data->objectForKey("scale"));
        if (num)
            scale=*num;
        else{
            Number * pScaleX((Number *)data->objectForKey("scaleX"));
            Number * pScaleY((Number *)data->objectForKey("scaleY"));
            
            if (pScaleX || pScaleY) {
                useScaleXY=true;
                if (pScaleX)
                    scaleX=*pScaleX;
                if (pScaleY)
                    scaleY=*pScaleY;
            }
            
        }
        
        if (data->objectForKey("anchorPoint"))
            node->setAnchorPoint(Functions::pointForObjectKey(data, "anchorPoint", node->getAnchorPoint()));
        
        if (data->objectForKey("contentSize"))
            node->setContentSize(Functions::sizeForObjectKey(data, "contentSize", node->getContentSize()));
        
        node->setPosition(pos);
        
        if (useScaleXY){
            node->setScaleX(scaleX);
            node->setScaleY(scaleY);
        }else
            node->setScale(scale);
        
        node->setRotation(rotation);
        
        if (data->objectForKey("visible"))
            node->setVisible(Functions::boolForObjectKey(data, "visible", node->isVisible()));
        
    }

    
    //color
    unsigned int _parseHex(const char *pszValue){
        unsigned int uiValue = 0;
        
        if (pszValue && strlen(pszValue) >= 6){
            for (int i = 0; i < 6; i++)
                if (pszValue[i] >= '0' && pszValue[i] <= '9')
                    uiValue = (uiValue << 4) | (pszValue[i] - '0');
                else if (pszValue[i] >= 'a' && pszValue[i] <= 'f')
                    uiValue = (uiValue << 4) | (pszValue[i] - 'a' + 10);
                else if (pszValue[i] >= 'A' && pszValue[i] <= 'A')
                    uiValue = (uiValue << 4) | (pszValue[i] - 'A' + 10);
                else
                    uiValue = uiValue << 4;
        }
        
        return uiValue;
    }
    
    cocos2d::ccColor3B parseWebRGB(const std::string webColor){
        const char *pszColor(webColor.c_str());
        unsigned int uiColor = 0;
        if (strlen(pszColor) > 1){
            if (pszColor[0] != '#')
                uiColor = atoi(pszColor + 1);
            else if (strlen(pszColor) >= 7)
                uiColor = _parseHex(pszColor + 1);
        }
        
        
        
        return cocos2d::ccColor3B{static_cast<GLubyte>(((uiColor >> 16) & 0xff)),
            static_cast<GLubyte>(((uiColor >> 8) & 0xff)),
            static_cast<GLubyte>(((uiColor)& 0xff))};
    }
    
}}}
