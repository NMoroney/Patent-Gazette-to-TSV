// © 2024 : github.com/NMoroney
// MIT License 
//

//
//   * https://developer.uspto.gov/product/patent-official-gazettes-listing
//   * https://github.com/frk1/ziplib/tree/master
//

#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

#include "ZipFile.h"
#include "streams/memstream.h"
#include "methods/Bzip2Method.h"


namespace n8m {

  using Strings = std::vector<std::string>;

  std::string version_gazette_to_tsv { "24.02" };


  // zip processing
  //
  Strings zip_list_files(const char* name_zip) {

    ZipArchive::Ptr archive = ZipFile::Open(name_zip);

    Strings names;
    size_t number_entries = archive->GetEntriesCount();
    for (size_t i = 0; i < number_entries; ++i) {
      auto entry = archive->GetEntry(int(i));
      if (!entry->IsDirectory())
        names.push_back(entry->GetFullName());
    }
    return names;
  }


  void zip_extract_file(const char* name_zip,
                        const char* name_file,
                        const char* name_out) {

    ZipFile::ExtractFile(name_zip, name_file, name_out);

  }


  std::string zip_file_to_string(ZipArchive::Ptr archive,
                                 const std::string& file_name) {

    std::istream* decompress_stream = nullptr;

    ZipArchiveEntry::Ptr entry = archive->GetEntry(file_name.c_str());
    assert(entry != nullptr);

    decompress_stream = entry->GetDecompressionStream();
    assert(decompress_stream != nullptr);

    char c;
    std::string content = "";
    while (decompress_stream->get(c)) {
       content += c;
    }
    return content;
  }


  // string processing
  //
  bool str_contains(const std::string& test, 
                    const std::string& query) {
    if (query.size() > test.size()) {
      return false;         
    } else {                
      for (auto i = 0; i < test.size(); ++i) {
        int match_count { 0 };
        for (auto j = 0; (j < query.size()) && ((i + j) < test.size()) ; ++j) {
          if (test[i + j] == query[j]) {
            ++match_count;
          }
        }
        if (match_count == query.size()) {
          return true;
        }
      }     
      return false;
    }
  }

  Strings str_split(const std::string& to_split,
                    const std::string& delimiter) {
    std::string s(to_split);
    Strings tokens;
    std::string token;

    size_t pos = 0;
    while ((pos = s.find(delimiter)) != std::string::npos) {
      token = s.substr(0, pos);
      tokens.push_back(token);
      s.erase(0, pos + delimiter.length());
    }

    if (s.size() > 0) {
      tokens.push_back(s);
    }

    return tokens;
  }

  bool str_starts_with(const std::string& test,
                       const std::string& prefix) {
    if (prefix.size() > test.size()) { return false; }
    for (auto i = 0; i < prefix.size(); ++i) {
      if (test[i] != prefix[i]) {
        return false;
      }
    }
    return true;
  }
    
  bool str_ends_with(const std::string& test,
                     const std::string& suffix) {
    if (suffix.size() > test.size()) { return false; }
    auto its = test.rbegin();
    auto itx = suffix.rbegin();
    while (itx != suffix.rend()) {
      if (*its != *itx) {   
        return false;
      }
      ++its;
      ++itx; 
    }
    return true;
  } 

  std::string str_to_replaced(const std::string& s,
                              const std::string& query,
                              const std::string& replacement) {
    std::string r;
    auto last { s.size() -  1 };
    for (auto i = 0; i < s.size(); ++i) {
      bool was_found { true };
      for (auto j = 0; (j < query.size()) && ((i + j) <= last); ++j) {
        if (s[i + j] != query[j]) {
          was_found = false;
          break;
        }
      }
      if (was_found && (i < last) ) {
        r += replacement;
        i += query.size() - 1;
      }
      if (!was_found) {
        r += s[i];
      }
    }
    return r;
  }


  // html processing
  //
  Strings list_html_files(const Strings& file_names) {
    std::string q1 { "OG/html" }, 
                q2 { ".html" },
                d1 { "/" },
                d2 { "-" };
    Strings html_files;
    for (auto name : file_names) {
      if (str_contains(name, q1) && str_ends_with(name, q2)) {
        auto ts = str_split(name, d1);
        html_files.push_back(name);
      }
    }
    return html_files;
  }

  void values_from_html(const Strings& lines,
                        std::string& number,
                        std::string& title,
                        std::string& inventor,
                        std::string& assigned) {
 
    std::string q1 { "table_data" },
                q2 { "Assigned" },
                s1 { "Assigned to " },
                d1 { "<b>" },
                d2 { "</b>" };
    int n { 0 };
    for (auto line : lines) {
      if (str_contains(line, q1)) {
        auto ts = str_split(line, d1);
        if (ts.size() > 1) {
          auto us = str_split(ts[1], d2);
          if (n == 0) { number    = us[0]; }
          if (n == 1) { title     = us[0]; }
          if (n == 2) { inventor  = us[0]; }
          if ((n == 3) && str_starts_with(us[0], q2)) { 
            assigned = str_to_replaced(us[0], s1, "");
          }
          if ((n == 4) && str_starts_with(us[0], q2)) { 
            assigned = str_to_replaced(us[0], s1, "");
          }
          ++n;
        }
      }
    }
  }


  void gazette_to_tsv_ntia(const char* path, const char* gazette) {
    using std::cout;
    cout << "gazette to tsv ntia :\n";

    std::string name { std::string { path } + std::string { gazette } };

    cout << "name : " << name << "\n";

    auto names = zip_list_files(name.c_str());
    auto htmls = list_html_files(names);

    cout << "htmls size : " << htmls.size() << "\n";

    ZipArchive::Ptr archive = ZipFile::Open(name.c_str());

    std::string name_out { gazette };
    for (int i = 0; i < 4; ++i) { name_out.pop_back(); }
    name_out += "_ntia.tsv";
    std::ofstream ofs(name_out);
    ofs << "number\ttitle\tinventor\tassigned\n";

    std::string number, title, inventor, assigned;
    for (auto html : htmls) {
      auto s = zip_file_to_string(archive, html.c_str());
      auto lines = str_split(s, "\n");
      values_from_html(lines, number, title, inventor, assigned);
      ofs << number << "\t" << title << "\t" << inventor << "\t" << assigned << "\n";
    }
  }

}

int main(int argc, char* argv[]) {

  if (argc == 1) {
    std::cout << "\n"
              << "gazette_to_tsv /path/to/zip file.zip\n"
              << "\n"
              << "for an input gazette as zip file extract the number, title,\n"
              << "inventors and assignment as TSV file.\n"
              << "\n";
  } else {
    n8m::gazette_to_tsv_ntia(argv[1], argv[2]);
  }

  return 0;

}
