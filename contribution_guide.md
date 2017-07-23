# Contributing to HandsOnLessons

## Previewing your changes locally

If you are new to GitHub and Jekyll themed GitHub pages, read this section to
learn how to preview your work locally before committing it to GitHub. On the
other hand, if its easier for you, you can just commit changes to GitHub, see
how they turn out there and modify if you are not satisified. That workflow
just takes a bit more time because GitHub/Jekyll backend may be delayed a
minute or two in re-generating the site. And, it also means your changes
are always going live.

This site is a GitHub pages site using a GitHub supported Jekyll theme backend.
This means you compose content in GitHub Markdown and when you commit changes to
GitHub, Jekyll generates the html pages for the site automatically. But, it also
means its a bit harder for you to preview, locally, your changes before committing.

To permit the site to be easily switched to another GitHub supported Jekyll theme,
I have chosen **not** to commit to the repository all the Jekyll accoutrements.
In any event, even if they were present, you would still wind up having to run
Jekyll to re-build and serve the site, each time you want to preview it.

To preview changes locally, you will need to run these commands

```
$ gem install github-pages
$ bundle exec jekyll serve
```

To get all the tools on my Mac (gem, bundle, jekyll), I used homebrew.

## Including Math Equations

I am not sure I have found the easiest solution here. I have read that MathJax may be
better but it seemed to require a bit more configuration than had time to play and
what I tried here seemed to work. The code snipit below demonstrates an example...

```
![equation](http://latex.codecogs.com/gif.latex?%5Cfrac%7B%5Cpartial%20Q%7D%7B%5Cpartial%20t%7D%20%3D%20%5Cfrac%7B%5Cpartial%20s%7D%7B%5Cpartial%20t%7D)
```

![equation](http://latex.codecogs.com/gif.latex?%5Cfrac%7B%5Cpartial%20Q%7D%7B%5Cpartial%20t%7D%20%3D%20%5Cfrac%7B%5Cpartial%20s%7D%7B%5Cpartial%20t%7D)

Everything after the question-mark charcter at the end of `latex?` and before the closing right-paranthesis
is the a URL-encoded form of the latex commands to generate the equation. Here are the steps...

1. Create the latex for the equation
2. URL-encode it using something like, [url-encode-decode](http://www.url-encode-decode.com)
3. Replace all instances of `+` that step 2 inserted to represent spaces with `%20`
4. Paste the resulting, possibly very long, string between `?` and enclosing `)` in the above.

## Each Hands On Example

I think it would be best put all content related to each hands-on example we develop here
into its own separate directory. That means all images, example codes, markdown pages, etc.
Then, we can have a sepate page (or page hierarchy) that indexes the examples.

I know Jekyll has a built-in concept of a `posts` object. Thats because Jekyll is designed
around the notion of supporting blogging. It may make sense to handle each hands-on kinda
sorta like a `post` in Jekyll. But, I think that also means that content related to each
lesson gets scattered across multiple directories (at least given the **default**) way that
Jekyll seems to handle `posts`.

## GitHub Style Primer

You can use the [editor on GitHub](https://github.com/xsdk-project/HandsOnLessons/edit/master/README.md) to maintain and preview the content for your website in Markdown files.

Whenever you commit to this repository, GitHub Pages will run [Jekyll](https://jekyllrb.com/) to rebuild the pages in your site, from the content in your Markdown files.

### Markdown

Markdown is a lightweight and easy-to-use syntax for styling your writing. It includes conventions for

```markdown
Syntax highlighted code block

# Header 1
## Header 2
### Header 3

- Bulleted
- List

1. Numbered
2. List

**Bold** and _Italic_ and `Code` text

[Link](url) and ![Image](src)
```

For more details see [GitHub Flavored Markdown](https://guides.github.com/features/mastering-markdown/).

### Jekyll Themes

Your Pages site will use the layout and styles from the Jekyll theme you have selected in your [repository settings](https://github.com/xsdk-project/HandsOnLessons/settings). The name of this theme is saved in the Jekyll `_config.yml` configuration file.

### Support or Contact

Having trouble with Pages? Check out our [documentation](https://help.github.com/categories/github-pages-basics/) or [contact support](https://github.com/contact) and we’ll help you sort it out.
