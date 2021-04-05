//
// Created by Chris Luttio on 3/28/21.
//

#include "url.h"
#include "utils.h"
#include <algorithm>

URL URL::parse(const std::string &str) {
    auto url = URL();

    if (str.empty()) {
        return url;
    }

    if (str == "/") {
        url.is_root = true;
        return url;
    }

    url.is_root = str[0] == '/';

    auto components = split_string("/", str);
    if (str[0] != '/') {
        if (components[0].find(':') != std::string::npos) {
            url.protocol = components[0].substr(0, components[0].size() - 1);
            components.erase(components.begin());
        }
    }

    for (auto component: components) {
        if (!component.empty()) {
            url.components.push_back(component);
        }
    }

    if (!url.components.empty() && !url.protocol.empty()) {
        auto first = url.components[0];
        if (first.find('.') != std::string::npos) {
            auto comps = split_string(".", first);
            std::reverse(comps.begin(), comps.end());
            url.domain = comps;
            url.components.erase(url.components.begin());
        }
    }

    if (!url.components.empty()) {
        auto last = *(url.components.end() - 1);
        if (last.find('.') != std::string::npos) {
            auto parts = split_string(".", last);
            if (parts.size() > 1) {
                url.components[url.components.size() - 1] = join(".", std::vector(parts.begin(), parts.end() - 1));
                url.extension = parts[parts.size() - 1];
            }
        }
    }

    return url;
}

URL URL::append(const URL &base, const URL &component) {
    auto url = URL();

    url.is_root = base.is_root;
    url.components = std::vector(base.components);
    url.domain = std::vector(base.domain);
    url.protocol = base.protocol;

    for (auto c: component.components) {
        url.components.push_back(c);
    }

    url.extension = component.extension;

    return url;
}

std::string URL::to_string() const {
    std::string output;

    if (is_root) {
        output += "/";
    }

    if (!protocol.empty()) {
        output += protocol + "://";
    }

    if (!domain.empty()) {
        std::vector<std::string> dp;
        for (auto iter = domain.rbegin(); iter != domain.rend(); iter++) {
            dp.push_back(*iter);
        }
        output += join(".", dp) + "/";
    }

    if (!components.empty()) {
        output += join("/", components);
    }

    if (!extension.empty()) {
        output += "." + extension;
    }

    return output;
}

const char* URL::domain_to_cstr() const {
    auto cpy_domain = std::vector(domain);
    std::reverse(cpy_domain.begin(), cpy_domain.end());
    auto str = join(".", cpy_domain);
    char* data = new char[str.size()];
    memcpy(data, str.c_str(), str.size());
    return data;
}

void URL::prepend(const std::string &str) {
    components.insert(components.begin(), str);
}

void URL::append(const std::string &str) {
    components.push_back(str);
}