# AFF3CT Documentation Generation

**AFF3CT** uses [Sphinx](http://www.sphinx-doc.org) to generate the documentation.

Install Sphinx using Python and pip:

```bash
sudo apt install python3 python3-pip librsvg2-bin
sudo pip3 install -r requirements.txt
```

To generate the documentation, you simply have to do:

```bash
make html
```

The last command will create a new folder: `build/html`.
In the `html` folder you just have to open the `index.html` file to read the generated documentation.