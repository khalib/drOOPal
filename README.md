drOOPal
=======

Object-oriented approach with helper classes for nodes, users, and forms for Drupal 7.

Why drOOPal?
------------
In a world where many if not most complex systems are based on a language that has Object Oriented Programming (OOP) in it's DNA, drOOPal can help take the procedural programming aspects of Drupal and incorporate using OOP patterns and practices (encapsulation, inheritance, abstraction, etc) with Drupal's fundamental entities (nodes, users, and forms) and maintain overall code that is Drupal compliant.

Usage
-----
Take an existing content type and make it OOP using drOOPal.  Let's take a simple blog with content type *blog*.

<pre>
class Blog extends Node {
	protected $node_type = 'blog';
}

// You can load a blog object using a nid or a node object.
$node = node_load(123);
$blog = new Blog($node);

// or

$blog = new Blog(123);

// Say you have a field named *field_blog_body* that you want to use.

// The non-drOOPal way.
$blog_body = NULL;
if (property_exists($node, 'field_blog_body')) {
  $entity = entity_metadata_wrapper('node', $node);
  if (!empty($entity)) {
  	$blog_body = $entity->field_blog_body->value();
  }
}
print $blog_body;

// The drOOPal way.
print $blog->blog_body;

</pre>

The drOOPal standard classes come with many helpful methods, which can all be extended in the custom classes:
<pre>
class Blog extends Node {
	protected $node_type = 'blog';
	
	public function get_asdf() {
	
	}
}

$node = node_load(123);
$blog = new Blog($node);

// Let's get the author info of the blog node, but first check if the blog is loaded.

// The non-drOOPal way.
if (!empty($node->nid)) {
	$user = user_load($node->uid);
	print $user->name;
}

// The drOOPal way.
if ($blog->is_loaded()) {
	// Returns a drOOPal User object.
	print $blog->get_user()->name;
}
</pre>