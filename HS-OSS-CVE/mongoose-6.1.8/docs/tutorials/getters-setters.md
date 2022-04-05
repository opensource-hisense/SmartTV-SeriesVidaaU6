# Getters/Setters in Mongoose

Mongoose getters and setters allow you to execute custom logic when getting or setting a property on a [Mongoose document](/docs/documents.html). Getters let you transform data in MongoDB into a more user friendly form, and setters let you transform user data before it gets to MongoDB.

## Getters

Suppose you have a `User` collection and you want to obfuscate user emails to protect your users' privacy. Below is a basic `userSchema` that obfuscates the user's email address.

```javascript
[require:getters/setters.*getters.*basic example]
```

Keep in mind that getters do **not** impact the underlying data stored in
MongoDB. If you save `user`, the `email` property will be 'ab@gmail.com' in
the database.

By default, Mongoose executes getters when converting a document to JSON,
including [Express' `res.json()` function](http://expressjs.com/en/4x/api.html#res.json).

```javascript
app.get(function(req, res) {
  return User.findOne().
    // The `email` getter will run here
    then(doc => res.json(doc)).
    catch(err => res.status(500).json({ message: err.message }));
});
```

To disable running getters when converting a document to JSON, set the [`toJSON.getters` option to `false` in your schema](https://mongoosejs.com/docs/guide.html#toJSON) as shown below.

```javascript
const userSchema = new Schema({
  email: {
    type: String,
    get: obfuscate
  }
}, { toJSON: { getters: false } });
```

To skip getters on a one-off basis, use [`user.get()` with the `getters` option set to `false`](/docs/api/document.html#document_Document-get) as shown below.

```javascript
[require:getters/setters.*getters.*skip]
```

## Setters

Suppose you want to make sure all user emails in your database are lowercased to 
make it easy to search without worrying about case. Below is an example
`userSchema` that ensures emails are lowercased.

```javascript
[require:getters/setters.*setters.*basic]
```

Mongoose also runs setters on update operations, like [`updateOne()`](/docs/api/query.html#query_Query-updateOne). Mongoose will
[upsert a document](https://masteringjs.io/tutorials/mongoose/upsert) with a
lowercased `email` in the below example.

```javascript
[require:getters/setters.*setters.*updates]
```

In a setter function, `this` can be either the document being set or the query
being run. If you don't want your setter to run when you call `updateOne()`,
you add an if statement that checks if `this` is a Mongoose document as shown
below.

```javascript
[require:getters/setters.*setters.*update skip]
```

Differences vs ES6 Getters/Setters
----------------------------------

Mongoose setters are different from [ES6 setters](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Functions/set) because they allow you to transform the value being set. With ES6 setters, you
would need to store an internal `_email` property to use a setter. With Mongoose,
you do **not** need to define an internal `_email` property or define a 
corresponding getter for `email`.

```javascript
[require:getters/setters.*setters.*vs ES6]
```