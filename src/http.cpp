/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvan-hum <dvan-hum@student.42perpignan.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:03:50 by dvan-hum          #+#    #+#             */
/*   Updated: 2025/03/26 16:03:22 by dvan-hum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"

HttpMethod getMethod(const string& method) {
	if (method == "GET")
		return GET;
	else if (method == "POST")
		return POST;
	else if (method == "DELETE")
		return DELETE;
	throw HttpRequestError(METHOD_NOT_ALLOWED);
}

string getHttpStatusMessage(HttpStatus status) {
	switch (status) {
	case CONTINUE: return "Continue";
	case SWITCHING_PROTOCOLS: return "Switching Protocols";
	case PROCESSING: return "Processing";
	case EARLY_HINTS: return "Early Hints";

	case OK: return "Ok";
	case CREATED: return "Created";
	case ACCEPTED: return "Accepted";
	case NON_AUTHORITATIVE_INFORMATION: return "Non Authoritative Information";
	case NO_CONTENT: return "No Content";
	case RESET_CONTENT: return "Reset Content";
	case PARTIAL_CONTENT: return "Partial Content";
	case MULTI_STATUS: return "Muti-Status";
	case ALREADY_REPORTED: return "Already Reported";
	case IM_USED: return "I'm Used";

	case MULTIPLE_CHOICES: return "Multiple Choices";
	case MOVED_PERMANENTLY: return "Moved Permanently";
	case FOUND: return "Found";
	case SEE_OTHER: return "See Other";
	case NOT_MODIFIED: return "Not Modified";
	case USE_PROXY: return "Use Proxy";
	case SWITCH_PROXY: return "Switch Proxy";
	case TEMPORARY_REDIRECT: return "Temporary Redirect";
	case PERMANENT_REDIRECT: return "Permanent Redirect";

	case BAD_REQUEST: return "Bad Request";
	case UNAUTHORIZED: return "Unauthorized";
	case PAYMENT_REQUIRED: return "Payment Required";
	case FORBIDDEN: return "Forbidden";
	case NOT_FOUND: return "Not Found";
	case METHOD_NOT_ALLOWED: return "Method Not Allowed";
	case NOT_ACCEPTABLE: return "Not Acceptable";
	case PROXY_AUTHENTICATION_REQUIRED: return "Proxy Authentication Required";
	case REQUEST_TIMEOUT: return "Request Timeout";
	case CONFLICT: return "Conflict";
	case GONE: return "Gone";
	case LENGTH_REQUIRED: return "Length Required";
	case PRECONDITION_FAILED: return "Precondition Failed";
	case PAYLOAD_TOO_LARGE: return "Payload Too Large";
	case URI_TOO_LONG: return "URI Too Long";
	case UNSUPPORTED_MEDIA_TYPE: return "Unsupported Media Type";
	case RANGE_NOT_SATISFIABLE: return "Range Not Satisfiable";
	case EXPECTATION_FAILED: return "Expectation Failed";
	case IM_A_TEAPOT: return "I'm A Teapot";
	case MISDIRECTED_REQUEST: return "Misdirected Request";
	case UNPROCESSABLE_CONTENT: return "Unprocessable Content";
	case LOCKED: return "Locked";
	case FAILED_DEPENDENCY: return "Failed Dependency";
	case TOO_EARLY: return "Too Early";
	case UPGRADE_REQUIRED: return "Upgrade Required";
	case PRECONDITION_REQUIRED: return "Precondition Required";
	case TOO_MANY_REQUESTS: return "Too Many Requests";
	case REQUEST_HEADER_FIEDLS_TOO_LARGE: return "Request Header Fields Too Large";
	case UNAVAILABLE_FOR_LEGAL_REASONS: return "Unavailable For Legal Reasons";

	case INTERNAL_SERVER_ERROR: return "Internal Server Error";
	case NOT_IMPLEMENTED: return "Not Implemented";
	case BAD_GATEWAY: return "Bad Gateway";
	case SERVICE_UNAVAILABLE: return "Service Unavailable";
	case GATEWAY_TIMEOUT: return "Gateway Timeout";
	case HTTP_VERSION_NOT_SUPPORTED: return "HTTP Version Not Supported";
	case VARIANT_ALSO_NEGOTIATES: return "Variant Also Negotiates";
	case INSUFFICIENT_STORAGE: return "Insufficient Storage";
	case LOOP_DETECTED: return "Loop Detected";
	case NOT_EXTENDED: return "Not Extended";
	case NETWORK_AUTHENTICATION_REQUIRED: return "Network Authentication Required";
	}
}

sockaddr_in parseAddress(const string &address) {
	string host;
	int port;

	size_t separator = address.find(':');
	
	if (separator >= address.length())
		host = "0.0.0.0";
	else
		host = address.substr(0, separator);
	
	char *end;
	port = strtol(address.substr(separator + 1).c_str(), &end, 10);
	if (*end != 0)
		throw runtime_error("Invalid address: " + address);
	if (port < 0 || port > 65535)
		throw range_error("Port not within range [0, 65535]: " + address);

	in_addr_t addr = 0;
	for (int i = 0; i < 4; i++) {
		separator = host.find('.');
		if (i < 3 && separator >= host.length())
			throw runtime_error("Invalid address: " + address);
		int addrPart = strtol(host.substr(0, separator).c_str(), &end, 10);
		if (*end != 0 || addrPart < 0 || addrPart > 255)
			throw runtime_error("Invalid address: " + address);
		addr |= addrPart << (8 * (3 - i));
		host = host.substr(host.find('.'));
	}

	return (sockaddr_in) {
		.sin_family = AF_INET,
		.sin_port = htons(port),
		.sin_addr = { .s_addr = addr }
	};
}
