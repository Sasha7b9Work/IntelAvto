/////////////////////////////////////////////////////////////////////////////
// Name:        envvars.h
// Purpose:     topic overview
// Author:      wxWidgets team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**

@page overview_envvars Environment Variables

This section describes all environment variables that affect execution of
wxWidgets programs.

@beginDefList
@itemdef{WXTRACE,
        (Debug build only.)
        This variable can be set to a comma-separated list of trace masks used in
        wxLogTrace calls; wxLog::AddTraceMask is called for every mask
        in the list during wxWidgets initialization.}
@itemdef{WXPREFIX,
        (Unix only.)
        Overrides installation prefix. Normally, the prefix
        is hard-coded and is the same as the value passed to @c configure via
        the @c \--prefix switch when compiling the library (typically
        @c /usr/local or @c /usr). You can set WXPREFIX if you are for example
        distributing a binary version of an application and you don't know in advance
        where it will be installed.}
@itemdef{WXWEBREQUEST_BACKEND,
         This variable can be set to override the choice of the default backend
         used by wxWebRequest, see wxWebSession::New(). Most common use is to
         set it to @c "CURL" to force using libcurl-based implementation under
         MSW or macOS platforms where the native implementation would be chosen
         by default.}
@itemdef{WXSUPPRESS_SIZER_FLAGS_CHECK,
         If set, disables asserts about using invalid sizer flags in the code.
         This can be helpful when running older programs recompiled with
         wxWidgets 3.1 or later, as these asserts are mostly harmless and can
         be safely ignored if the code works as expected.}
@itemdef{WXLANGUAGE,
         This variable can be set to override OS setting of preferred languages
         and make wxUILocale::GetPreferredUILanguages() return the set list
         instead. The format is same as GNU's <a
         href="https://www.gnu.org/software/gettext/manual/html_node/The-LANGUAGE-variable.html">LANGUAGE</a>
         variable: a colon-separated list of language codes.}
*/

