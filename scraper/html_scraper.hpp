#ifndef  __KOAI_HTML_SCRAPER__
#define  __KOAI_HTML_SCRAPER__

#include <libxml2/libxml/HTMLparser.h>
#include <libxml2/libxml/xpath.h>
#include <libxml2/libxml/parser.h>
#include <vector>

namespace html{
  struct Image{
    std::string url;
    std::string width, height;
    Image();
    Image(const Image&)=delete;
    Image operator=(const Image&)=delete;
    Image(Image&&)=default;
    Image& operator=(Image&&)=default;
  };

  struct HtmlElement{
    xmlNode*element;
    std::string text();
    HtmlElement(xmlNode*elem);
  };
}

namespace html{
    namespace class_attrs{//ufc htmls classes attributes.
        constexpr auto DEVISION_CLASS = "hero-profile__division-title";
        constexpr auto ACCURACY_CARDS ="overlap-athlete-content overlap-athlete-content--horizontal";
        constexpr auto ACCURACY  ="e-chart-circle__percent";
        constexpr auto COMPARE_CARDS ="c-stat-compare__group c-stat-compare__group-";
        constexpr auto COMAPRE_VALUE ="c-stat-compare__number";
        constexpr auto POS_CARD  ="c-stat-3bar__group";
        constexpr auto POS_VALUE ="c-stat-3bar__value";
        constexpr auto TARGET_HEAD_ID ="e-stat-body_x5F__x5F_head_value";
        constexpr auto TARGET_BODY_ID ="e-stat-body_x5F__x5F_body_value";
        constexpr auto TARGET_LEG_ID ="e-stat-body_x5F__x5F_leg_value";
        constexpr auto WIN_BY_METHOD_CARD ="c-stat-3bar__group";
        constexpr auto WIN_BY_METHOD_VALUE ="c-stat-3bar__value";
        constexpr auto STATUS_CARD ="c-bio__field";
        constexpr auto STATUS_VALUE ="c-bio__text";
        constexpr auto BIO_CARDS  ="c-bio__field c-bio__field--border-bottom-small-screens";
        constexpr auto BIO_CARD_TEXT ="c-bio__text";
        //fighting status 
        constexpr auto NOT_FIGHTING_STATUS_TXT ="Not Fighting";
        //unknown info
        constexpr auto UNKNOWN_INFO ="none";
    }
  class HtmlScraper{
    enum BY{CLASS_NAME,ID};
    std::string htmlSrc;
    htmlDocPtr doc;
    std::string getClassAttrName(BY by);
    void doFreeDoc();
    public:
    HtmlScraper();
    HtmlScraper(const HtmlScraper&)=delete;
    HtmlScraper&operator=(const HtmlScraper&)=delete;
    HtmlScraper(HtmlScraper&&)=delete;
    HtmlScraper&operator=(HtmlScraper&&)=delete;      
    void setHtml(std::string&& html);
    std::vector<html::HtmlElement> getElements(xmlNode* parent, const std::string& className,BY by);
    std::vector<html::HtmlElement> getElementsByClassName(xmlNode* parent, const std::string& className);
    std::vector<html::HtmlElement> getElementsByClassName(std::string className);
    std::vector<html::HtmlElement> getElementsById(xmlNode* parent, const std::string& id);
    std::vector<html::HtmlElement> getElementsById(std::string id);
    std::string getTextInChildElementByKeyword(std::string&keyword, std::string parentClass, std::string childClass);
    ~HtmlScraper();
};
}
#endif // ! __KOAI_HTML_SCRAPER__ INCLUDED