#!/bin/bash -x

# write to file
bootstrap_template()
{
    pandoc --standalone --toc --template=elegant_bootstrap_menu.html tutorial.md -o index.html --metadata title="PopJSON: JSON representation for structured population models"
}

# execute it
bootstrap_template