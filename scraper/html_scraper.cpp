#include "html_scraper.hpp"

html::Image::Image(){
    url=std::string{};
    width=std::string{};
    height=std::string{};
} 

html::HtmlElement::HtmlElement(xmlNode*elem):element(elem){}

std::string html::HtmlElement::text(){
    std::string txt{};
    if(!element){return txt;};
    xmlChar* content = xmlNodeGetContent(element);
    if(content) {
    txt.append((const char*)content);
    xmlFree(content);
    }
    return std::move(txt);
}

html::HtmlScraper::HtmlScraper(){
    doc=nullptr;
    htmlSrc=std::string{};
}

void html::HtmlScraper::doFreeDoc(){
    if(doc!=nullptr){
        xmlFreeDoc(doc);
        doc=nullptr;
    }
}

void html::HtmlScraper::setHtml(std::string&& html){
    doFreeDoc();
    htmlSrc=std::move(html);
    doc = htmlReadDoc(reinterpret_cast<const xmlChar*>(htmlSrc.c_str()), nullptr, "UTF-8", HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);
}

std::string html::HtmlScraper::getClassAttrName(BY by){
   switch (by)
    {
    case BY::CLASS_NAME:
        return "class";
    case BY::ID:
        return "id";
    default:
        break;
    }
    return "class";
}

/*std::vector<html::HtmlElement> html::HtmlScraper::getElements(xmlNode* parent, const std::string& attrName,BY by){
    std::vector<html::HtmlElement> matchingElements;
    constexpr size_t RESERVED_RES_SIZE = 256;
    matchingElements.reserve(RESERVED_RES_SIZE);
    std::vector<xmlNode*> stack; // Use a stack for iterative traversal
    constexpr size_t RESERVED_STK_SIZE = 1024;
    stack.reserve(RESERVED_STK_SIZE);
    std::string byAttr =getClassAttrName(by);
    const xmlChar* targetAttr = BAD_CAST byAttr.c_str(); // Store in string to maintain lifetime

    // Start with the parent node
    if(parent){
        stack.push_back(parent);
    }

    // Perform DFS-like traversal using the stack
    while (!stack.empty()) {
        xmlNode* current = stack.back();
        stack.pop_back();
       
        if (current->type == XML_ELEMENT_NODE) {
            // Check if the node has a class attribute
            xmlChar* classAttr = xmlGetProp(current, targetAttr);
            if (classAttr) {
                std::string classAttrStr = reinterpret_cast<const char*>(classAttr);
                xmlFree(classAttr);
                if (classAttrStr==attrName) {
                    matchingElements.push_back(current); // Add matching node
                }
            }
        }

        // Add all child nodes to the stack
        for (xmlNode* child = current->children; child; child = child->next) {
            if(child){
                stack.push_back(child);
            }
        }
    }

    return matchingElements;
}*/


std::vector<html::HtmlElement> html::HtmlScraper::getElements(xmlNode* parent, const std::string& attrName, BY by) {
    // Use a vector instead of a C-style array for the traversal stack
    std::vector<xmlNode*> nodeStack{};
    // Reserve capacity to avoid frequent reallocations
    constexpr size_t RESERVED_STK_SIZE = 1024;
    nodeStack.reserve(RESERVED_STK_SIZE);
    
    // Pre-allocate result vector with a reasonable capacity
    std::vector<html::HtmlElement> results{};
    constexpr size_t RESERVED_RES_SIZE = 256;
    results.reserve(RESERVED_RES_SIZE); // Increased to accommodate larger result sets
    
    // Convert attribute names to xmlChar* once
    const std::string attrNameStr = getClassAttrName(by);
    const xmlChar* targetAttr = BAD_CAST attrNameStr.c_str(); // Store in string to maintain lifetime
    const xmlChar* targetValue = BAD_CAST attrName.c_str();
    
    // Start traversal
    if (parent) {
        nodeStack.push_back(parent);
    }
    
    while (!nodeStack.empty()) {
        // Pop node from the back
        xmlNode* node = nodeStack.back();
        nodeStack.pop_back();
        
        //if (!node) continue; // Skip null nodes
        
        // Process only element nodes
        if (node->type == XML_ELEMENT_NODE) {
            // Get property safely
            xmlChar* attrValue = xmlGetProp(node, targetAttr);
            
            // Check for matching attribute
            if (attrValue && xmlStrcmp(attrValue, targetValue) == 0) {
                // Create a new HtmlElement and add to results
                results.emplace_back(node);
            }
            
            // Free memory immediately
            if (attrValue) {
                xmlFree(attrValue);
                attrValue = nullptr;
            }
            
            // Add children to stack (in reverse for proper traversal)
            // Check each child for validity before adding
            for (xmlNode* child = node->last; child; child = child->prev) {
                if(child)
                nodeStack.push_back(child);
            }
        }
    }
    
    return results;
}


std::vector<html::HtmlElement> html::HtmlScraper::getElementsByClassName(xmlNode* parent, const std::string& className) {
    return getElements(parent, className,BY::CLASS_NAME);
}

std::vector<html::HtmlElement>html::HtmlScraper::getElementsByClassName(std::string className){
    std::vector<html::HtmlElement>elements{};
   if(doc){
    xmlNode* root = xmlDocGetRootElement(doc);
    elements=getElementsByClassName(root,className);
   }
   return elements;
}


std::vector<html::HtmlElement>html::HtmlScraper::getElementsById(xmlNode* parent, const std::string& id){
    return getElements(parent, id,BY::ID);
}

std::vector<html::HtmlElement> html::HtmlScraper::getElementsById(std::string id){
    std::vector<html::HtmlElement>elements{};
   if(doc){
    xmlNode* root = xmlDocGetRootElement(doc);
    elements=getElementsById(root,id);
   }
   return elements;
}

std::string html::HtmlScraper::getTextInChildElementByKeyword(std::string&keyword, std::string parentClass, std::string childClass){
    auto parentElements = getElementsByClassName(parentClass);
    for(auto&childElement:parentElements){
        if(childElement.text().find(keyword)!=std::string::npos){
            auto foundElements = getElementsByClassName(childElement.element,childClass);
            if(foundElements.size()>0){
              std::string firstElement =foundElements[0].text();
              return firstElement;
            }
        }
    }
    return std::string{};
}

html::HtmlScraper::~HtmlScraper(){
    doFreeDoc();
    xmlCleanupParser();
}