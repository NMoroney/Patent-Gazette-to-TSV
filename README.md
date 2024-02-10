
# Patent Gazette to TSV

Directly transform USPTO Patent Gazettes as .zip file to a TSV file

---

The USPTO Patent Gazettes can be downloaded as a .zip file from [this website](https://developer.uspto.gov/product/patent-official-gazettes-listing).

The Gazettes are described by the USPTO as :

> Published each Tuesday, the Patent Official Gazette contains bibliographic (front page) information, a representative claim, and a drawing (if applicable) of each patent grant issued that week. Includes U.S. Patent and Trademark Office (USPTO) Notices which provide important information and changes in rules concerning both patents and trademarks.

The Gazettes are a useful reference for the latest patents. However, each Gazette is rather sizeable and mixture of different file formats. The Feb 6, 2024 Gazette is over 200 MB in size and decompressed consists of over 20,000 individual files.

This repository uses [ziplib](https://github.com/frk1/ziplib/tree/master?tab=readme-ov-file) (originally on [bitbucket](https://bitbucket.org/wbenny/ziplib)) to directly transform an input Gazette zip to a file of Tab Separated Values or TSV file.

---
### Versions

**24.02** : Generate a TSV that consists only of the number, title, inventor(s) and assignment. Example result as [zipped](https://github.com/NMoroney/Patent-Gazette-to-TSV/blob/main/e-OG20240206_1519-1_ntia.tsv.zip). This reduces the number of files from +22,000 to a single file. In addition, the zipped file is ~0.5 MB while the original gazette zip is 220 MB.

