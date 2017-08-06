# Contributing to HandsOnLessons

## About GitHub Pages and Jekyll

[Jekyll](https://jekyllrb.com) allows site content developers to build beautiful
mostly static sites by composing ASCII files involving a combination of three
technologies...

* [Markdown](https://guides.github.com/features/mastering-markdown/)
(or [Textile](https://www.promptworks.com/textile)),
* YAML [Front Matter](http://jekyllrb.com/docs/frontmatter/) page configuration code
* [Liquid](https://shopify.github.io/liquid/) content filtering and page construction code

The Jekyll engine reads source `.md` files and, optionally, a number of
other CSS, HTML (and other Web technology code snippets and accouterments) stored in
supporting files and directories in the repository (when necessary) and builds the
site HTML files.

On a GitHub Pages site, this process happens automatically upon each new commit of
files to the site's repository. GitHub uses Jekyll to re-generate the site and the
changes go live shortly thereafter. See below about how to preview changes to the
site before committing them.

Within the repository, a single source `.md` file will contain not only Markdown
content, but may also contain, optionally, a YAML code block at the beginning of
the file (called its _front matter_) which holds information on how the page is
to be configured when the site is built and, optionally, Liquid instructions
which can appear anywhere in the file and which program the Jekyll engine on how
to filter, merge and combine content snippets into a generated page.

So far, we are not using either YAML Front Matter or Liquid here to build our
site. We may eventually decide we might need to do that and it would be fine
but there isn't anything yet that has indicated we have a need for that. I
mention this because we can easily get much more sophisticated than we are
currently in managing and hosting this content.

## Previewing your changes locally

If you are new to GitHub and Jekyll themed GitHub pages, read this section to
learn how to preview your work locally before committing it to GitHub. On the
other hand, if its easier for you, you can just commit changes to GitHub, see
how they turn out there and modify if you are not satisfied. That workflow
just takes a bit more time because GitHub/Jekyll backend may be delayed a
minute or two in re-generating the site. And, it also means your changes
are always going live.

This site is a GitHub pages site using a GitHub [supported](https://pages.github.com/themes/)
Jekyll theme backend. This means you compose content in GitHub Markdown and when you commit
changes to GitHub, Jekyll generates the HTML pages for the site automatically. But, it also
means its a bit harder for you to preview, locally, your changes before committing.

But, if we don't like the current theme we're using,
[Cayman](https://pages-themes.github.io/cayman/), we can easily change
it by going to the repository's settings page on github, scrolling down to the
GitHub Pages section and hitting the `Change Theme` button and then selecting
another theme. Note, there are only a handful of themes supported by GitHub this
way (e.g. easily switchable via the `Change Theme` button). However, that does not
mean we cannot choose from another UNsupported theme. There are hundreds of
[Jekyll themes](http://jekyllthemes.org) available and we should probably spend
some more time to find best. Using an UNsupported theme simply means that we'd have
to buy into committing all the Jekyll accouterments to our repo and, as a result,
it is not as easy to switch the theme later if we don't like it.

To permit the site to be easily switched to another GitHub supported Jekyll theme,
I have chosen **not** to commit to the repository all the Jekyll accouterments.
In any event, even if they were present, you would still wind up having to run
Jekyll to re-build and serve the site, each time you want to preview it.
To preview changes locally, you will need to run these commands

```
$ gem install github-pages
$ bundle exec jekyll serve
```

To get all the tools on my Mac (gem, bundle, jekyll), I used Homebrew.

Detailed instructions on setting up GitHub pages locally can be found [here](how_to_preview_locally.md).

## Including Math Equations

I am not sure I have found the easiest solution here. I have read that MathJax may be
better but it seemed to require a bit more configuration than had time to play and
what I tried here seemed to work. The code snippet below demonstrates an example...

```
![equation](http://latex.codecogs.com/gif.latex?%5Cfrac%7B%5Cpartial%20Q%7D%7B%5Cpartial%20t%7D%20%3D%20%5Cfrac%7B%5Cpartial%20s%7D%7B%5Cpartial%20t%7D)
```

![equation](http://latex.codecogs.com/gif.latex?%5Cfrac%7B%5Cpartial%20Q%7D%7B%5Cpartial%20t%7D%20%3D%20%5Cfrac%7B%5Cpartial%20s%7D%7B%5Cpartial%20t%7D)

Everything after the question-mark character at the end of `latex?` and before the closing right-parenthesis
is the a URL-encoded form of the latex commands to generate the equation. Here are the steps...

1. Create the latex for the equation
2. URL-encode it using something like, [url-encode-decode](http://www.url-encode-decode.com)
3. Replace all instances of `+` that step 2 inserted to represent spaces with `%20`
4. Paste the resulting, possibly very long, string between `?` and enclosing `)` in the above.

## Adding a Hands On Example

1. Add a new directory to `./lessons/<new-example-name>`
1. Add a new line to `./lessons/lessons.md` for the new example
1. Copy `./lessons/lesson_template/lesson.md` to `./lessons/<new-example-name>`.
1. Edit/revise the copied `lesson.md` file to create the new lesson
1. You can link to images, source code files that you place in `./lessons/<new-example-name>`
   using standard Markdown links.

I think it would be best put all content related to each hands-on example we develop here
into its own separate directory. That means all images, example codes, markdown pages, etc.
Then, we can have a separate page (or page hierarchy) that indexes the examples.

I know Jekyll has a built-in concept of a `posts` object. That is because Jekyll is designed
around the notion of supporting blogging. It may make sense to handle each hands-on kinda
sorta like a `post` in Jekyll. But, I think that also means that content related to each
lesson gets scattered across multiple directories (at least given the **default**) way that
Jekyll seems to handle `posts`.

## GitHub Style Primer

This section is just a copy of boilerplate content from GitHub Pages template
about how to use Markdown, etc. I have kept it here for convenience.

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
