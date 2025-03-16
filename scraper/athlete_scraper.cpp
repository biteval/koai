#include "athlete_scraper.hpp"

// helper fucntion to get a string which is a part of html class name
std::string athlete_scraper::AthleteScraper::getCompareCards1(std::string x)
{
    return x + "1 ";
}
// helper fucntion to get a string which is a part of html class name
std::string athlete_scraper::AthleteScraper::getCompareCards2(std::string x)
{
    return x + "2 ";
}

// athletes count in text payload
size_t athlete_scraper::AthleteScraper::getAthletsCount()
{
    return yextReader.getAthletsCount();
}
// check if the yext payload contains the key RESULTS_KEY.
bool athlete_scraper::AthleteScraper::isResultsExists()
{
    return yextReader.isResultsExists();
}

// check if the yext payload is empty.
bool athlete_scraper::AthleteScraper::isYextPayloadEmpty()
{
    return yextReader.isPayloadEmpty();
}

size_t athlete_scraper::AthleteScraper::getCurrResultsSize()
{
    return yextReader.currResultsSize();
}

int athlete_scraper::AthleteScraper::getSearchLImit()
{
    return yextReader.getSearchLImit();
}

utils::StringNormalizer &athlete_scraper::AthleteScraper::getStringNormilizer()
{
    return strNormilizer;
}

void athlete_scraper::AthleteScraper::setHtml(std::string &&src)
{
    htmlScraper.setHtml(std::move(src));
}

void athlete_scraper::AthleteScraper::parse(std::string &&yextSrc)
{
    yextReader.parse(std::move(yextSrc));
}

std::string athlete_scraper::AthleteScraper::getLandPageUrl()
{
    return yextReader.getLandPageUrl();
}

// set target athlete index yextReader payload
athlete_scraper::AthleteScraper &athlete_scraper::AthleteScraper::setTargetIndex(size_t &index)
{
    yextReader.setTargetAthleteIndex(static_cast<int>(index));
    return *this;
}

std::string athlete_scraper::AthleteScraper::getFirstName()
{
    std::string fname = yextReader.get(yext::yext_json_keys::YEXT_FIRST_NAME, false, strNormilizer);
    //remove spaces , names with multiple words becomes with one word only.
    strNormilizer.removeSpaces(fname);
    return fname;
}

std::string athlete_scraper::AthleteScraper::getLastName()
{
    return yextReader.get(yext::yext_json_keys::YEXT_LAST_NAME, false, strNormilizer);
}

std::string athlete_scraper::AthleteScraper::getDivision()
{
    auto elems = htmlScraper.getElementsByClassName(html::class_attrs::DEVISION_CLASS);
    std::string div{};
    if (!elems.empty())
    {
        div = elems[0].text();
    }
    strNormilizer.normalizeText(div);
    return !div.empty() ? div.substr(0, div.find(" ")) : html::class_attrs::UNKNOWN_INFO;
}

std::string athlete_scraper::AthleteScraper::getNickName()
{
    std::string nickName = yextReader.get(yext::yext_json_keys::NICK_NAME_KEY, false, strNormilizer);
    return !nickName.empty() ? nickName : std::string{};
}

std::string athlete_scraper::AthleteScraper::getStrikingAccuracy()
{
    std::string keyWord = "Striking accuracy";
    std::string val = htmlScraper.getTextInChildElementByKeyword(keyWord, html::class_attrs::ACCURACY_CARDS, html::class_attrs::ACCURACY);
    if (val.empty())
    {
        return std::string("0");
    }
    // remove percent sign
    strNormilizer.removePerCentSign(val);
    return val;
}

std::string athlete_scraper::AthleteScraper::getTakedownAccuracy()
{
    std::string keyWord = "Takedown Accuracy";
    std::string val = htmlScraper.getTextInChildElementByKeyword(keyWord, html::class_attrs::ACCURACY_CARDS, html::class_attrs::ACCURACY);
    if (val.empty())
    {
        return std::string("0");
    }
    // remove percent sign
    strNormilizer.removePerCentSign(val);
    return val;
}

std::string athlete_scraper::AthleteScraper::getSigStrLandedPerMin()
{
    std::string keyWord = "Landed";
    std::string val = htmlScraper.getTextInChildElementByKeyword(keyWord, getCompareCards1(html::class_attrs::COMPARE_CARDS), html::class_attrs::COMAPRE_VALUE);
    if (val.empty())
    {
        return std::string("0.0");
    }
    strNormilizer.removeSpaces(val);
    return val;
}

std::string athlete_scraper::AthleteScraper::getSigStrAbsorbedPerMin()
{
    std::string keyWord = "Absorbed";
    std::string val = htmlScraper.getTextInChildElementByKeyword(keyWord, getCompareCards2(html::class_attrs::COMPARE_CARDS), html::class_attrs::COMAPRE_VALUE);
    if (val.empty())
    {
        return std::string("0.0");
    }
    strNormilizer.removeSpaces(val);
    return val;
}

std::string athlete_scraper::AthleteScraper::getTakedownAvgPer15Min()
{
    std::string keyWord = "Takedown avg";
    std::string val = htmlScraper.getTextInChildElementByKeyword(keyWord, getCompareCards1(html::class_attrs::COMPARE_CARDS), html::class_attrs::COMAPRE_VALUE);
    if (val.empty())
    {
        return std::string("0.0");
    }
    strNormilizer.removeSpaces(val);
    return val;
}

std::string athlete_scraper::AthleteScraper::getSigStrDefense()
{
    std::string keyWord = "Sig. Str. Defense";
    std::string val = htmlScraper.getTextInChildElementByKeyword(keyWord, getCompareCards1(html::class_attrs::COMPARE_CARDS), html::class_attrs::COMAPRE_VALUE);
    if (val.empty())
    {
        return std::string("0");
    }
    // remove any spaces
    strNormilizer.removeSpacesFromNumberStr(val);
    // val.push_back('%');
    strNormilizer.removePerCentSign(val);
    return val;
}

std::string athlete_scraper::AthleteScraper::getKnockdownAvg()
{
    std::string keyWord = "Knockdown Avg";
    std::string val = htmlScraper.getTextInChildElementByKeyword(keyWord, getCompareCards1(html::class_attrs::COMPARE_CARDS), html::class_attrs::COMAPRE_VALUE);
    if (val.empty())
    {
        return std::string("0.0");
    }
    strNormilizer.removeSpaces(val);
    return val;
}

std::string athlete_scraper::AthleteScraper::getSubmissionAvgPer15Min()
{
    std::string keyWord = "Submission avg";
    std::string val = htmlScraper.getTextInChildElementByKeyword(keyWord, getCompareCards2(html::class_attrs::COMPARE_CARDS), html::class_attrs::COMAPRE_VALUE);
    if (val.empty())
    {
        return std::string("0.0");
    }
    strNormilizer.removeSpaces(val);
    return val;
}

std::string athlete_scraper::AthleteScraper::getTakeDownDefense()
{
    std::string keyWord = "Takedown Defense";
    std::string val = htmlScraper.getTextInChildElementByKeyword(keyWord, getCompareCards2(html::class_attrs::COMPARE_CARDS), html::class_attrs::COMAPRE_VALUE);
    if (val.empty())
    {
        return std::string("0");
    }
    strNormilizer.removeSpacesFromNumberStr(val);
    // val.push_back('%');
    strNormilizer.removePerCentSign(val);
    return val;
}

std::string athlete_scraper::AthleteScraper::getAverageFightTimeAsMinuts()
{
    std::string keyWord = "Average fight time";
    std::string val = htmlScraper.getTextInChildElementByKeyword(keyWord, getCompareCards2(html::class_attrs::COMPARE_CARDS), html::class_attrs::COMAPRE_VALUE);
    if (val.empty())
    {
        return std::string("0.0");
    }
    // convert to minuts double
    // double mins = htmlScraper.timeStrToMinuts(val);

    strNormilizer.removeSpaces(val);
    strNormilizer.timeToStr(val);
    return val;
}

std::string athlete_scraper::AthleteScraper::getSigStrByPosStanding()
{
    std::string keyWord = "Standing ";
    std::string val = htmlScraper.getTextInChildElementByKeyword(keyWord, html::class_attrs::POS_CARD, html::class_attrs::POS_VALUE);
    if (val.empty())
    {
        return std::string("0");
    }
    // get the int part
    val = val.substr(0, val.find(" "));
    return val;
}

std::string athlete_scraper::AthleteScraper::getSigStrByPosClinch()
{
    std::string keyWord = "Clinch ";
    std::string val = htmlScraper.getTextInChildElementByKeyword(keyWord, html::class_attrs::POS_CARD, html::class_attrs::POS_VALUE);
    if (val.empty())
    {
        return std::string("0");
    }
    // get the int part
    val = val.substr(0, val.find(" "));
    return val;
}

std::string athlete_scraper::AthleteScraper::getSigStrByPosGround()
{
    std::string keyWord = "Ground ";
    std::string val = htmlScraper.getTextInChildElementByKeyword(keyWord, html::class_attrs::POS_CARD, html::class_attrs::POS_VALUE);
    if (val.empty())
    {
        return std::string("0");
    }
    // get the int part
    val = val.substr(0, val.find(" "));
    return val;
}

std::string athlete_scraper::AthleteScraper::getSigStrByTargetHead()
{
    auto targetHeadElems = htmlScraper.getElementsById(html::class_attrs::TARGET_HEAD_ID);
    if (targetHeadElems.size() > 0)
    {
        std::string targetHeadVal = targetHeadElems[0].text();
        if (targetHeadVal.empty())
        {
            return std::string("0");
        }
        return targetHeadVal;
    }
    return std::string("0");
}

std::string athlete_scraper::AthleteScraper::getSigStrByTargetBody()
{
    auto targetHeadElems = htmlScraper.getElementsById(html::class_attrs::TARGET_BODY_ID);
    if (targetHeadElems.size() > 0)
    {
        std::string targetBodyVal = targetHeadElems[0].text();
        if (targetBodyVal.empty())
        {
            return std::string("0");
        }
        return targetBodyVal;
    }
    return std::string("0");
}

std::string athlete_scraper::AthleteScraper::getSigStrByTargetLeg()
{
    auto targetHeadElems = htmlScraper.getElementsById(html::class_attrs::TARGET_LEG_ID);
    if (targetHeadElems.size() > 0)
    {
        std::string targetLegVal = targetHeadElems[0].text();
        if (targetLegVal.empty())
        {
            return std::string("0");
        }
        return targetLegVal;
    }
    return std::string("0");
}

std::string athlete_scraper::AthleteScraper::getWinByKoTko()
{
    std::string keyWord = "KO/TKO ";
    std::string val = htmlScraper.getTextInChildElementByKeyword(keyWord, html::class_attrs::WIN_BY_METHOD_CARD, html::class_attrs::WIN_BY_METHOD_VALUE);
    if (val.empty())
    {
        return std::string("0");
    }
    // get the int part
    val = val.substr(0, val.find(" "));
    return val;
}

std::string athlete_scraper::AthleteScraper::getWinByDec()
{
    std::string keyWord = "DEC ";
    std::string val = htmlScraper.getTextInChildElementByKeyword(keyWord, html::class_attrs::WIN_BY_METHOD_CARD, html::class_attrs::WIN_BY_METHOD_VALUE);
    if (val.empty())
    {
        return std::string("0");
    }
    // get the int part
    val = val.substr(0, val.find(" "));
    return val;
}

std::string athlete_scraper::AthleteScraper::getWinBySub()
{
    std::string keyWord = "SUB ";
    std::string val = htmlScraper.getTextInChildElementByKeyword(keyWord, html::class_attrs::WIN_BY_METHOD_CARD, html::class_attrs::WIN_BY_METHOD_VALUE);
    if (val.empty())
    {
        return std::string("0");
    }
    // get the int part
    val = val.substr(0, val.find(" "));
    return val;
}

std::string athlete_scraper::AthleteScraper::getStatus()
{
    std::string keyWord = "Status";
    std::string val = htmlScraper.getTextInChildElementByKeyword(keyWord, html::class_attrs::STATUS_CARD, html::class_attrs::STATUS_VALUE);
    strNormilizer.normalizeText(val);
    return val;
}

std::string athlete_scraper::AthleteScraper::getFightingStyle()
{
    std::string keyWord = "Fighting style";
    std::string val = htmlScraper.getTextInChildElementByKeyword(keyWord, html::class_attrs::BIO_CARDS, html::class_attrs::BIO_CARD_TEXT);
    strNormilizer.normalizeText(val);
    return val;
}

std::string athlete_scraper::AthleteScraper::getAge()
{
    std::string age = yextReader.get(yext::yext_json_keys::AGE_kEY, false, strNormilizer);
    return age;
}

std::string athlete_scraper::AthleteScraper::getHeight()
{
    std::string height = yextReader.get(yext::yext_json_keys::HEIGHT_KEY, false, strNormilizer, false);
    // printf("HEIGHT=%s\n",height.c_str());
    return height;
}

// pounds
std::string athlete_scraper::AthleteScraper::getWeight()
{
    std::string weight = yextReader.get(yext::yext_json_keys::WEIGHT_KEY, false, strNormilizer, false);
    // printf("WEIGHT=%s\n",weight.c_str());
    return weight;
}

std::string athlete_scraper::AthleteScraper::getGender()
{
    std::string gender = yextReader.get(yext::yext_json_keys::GENDER_KEY, false, strNormilizer);
    return gender;
}

std::string athlete_scraper::AthleteScraper::getWeightClass()
{
    std::string weightClass = yextReader.get(yext::yext_json_keys::WEIGHT_CLASS_KEY, false, strNormilizer);
    return !weightClass.empty() ? weightClass : html::class_attrs::UNKNOWN_INFO;
}

html::Image athlete_scraper::AthleteScraper::getImage()
{
    std::string imgStrObj = yextReader.get(yext::yext_json_keys::IMAGE_KEY, true, strNormilizer);
    nlohmann::json imgJson{};
    // handle the case when no image provided for the athlete.
    if (imgStrObj.empty())
    {
        return html::Image{};
    }
    imgJson = nlohmann::json::parse(imgStrObj);
    html::Image img{};
    if (imgJson.contains(yext::yext_json_keys::IMAGE_URL_KEY))
    {
        img.url = imgJson[yext::yext_json_keys::IMAGE_URL_KEY];
    }
    if (imgJson.contains(yext::yext_json_keys::IMAGE_WIDTH_KEY) &&
        imgJson.contains(yext::yext_json_keys::IMAGE_HEIGHT_KEY))
    {
        img.width = imgJson[yext::yext_json_keys::IMAGE_WIDTH_KEY].dump();
        img.height = imgJson[yext::yext_json_keys::IMAGE_HEIGHT_KEY].dump();
    }
    return std::move(img);
}