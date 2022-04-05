# Transactions in Mongoose

[Transactions](https://www.mongodb.com/transactions) are new in MongoDB
4.0 and Mongoose 5.2.0. Transactions let you execute multiple operations
in isolation and potentially undo all the operations if one of them fails.
This guide will get you started using transactions with Mongoose.

<h2 id="getting-started-with-transactions"><a href="#getting-started-with-transactions">Getting Started with Transactions</a></h2>

If you haven't already, import mongoose:
```javascript
import mongoose from 'mongoose';
```

To create a transaction, you first need to create a session using or [`Mongoose#startSession`](/docs/api/mongoose.html#mongoose_Mongoose-startSession)
or [`Connection#startSession()`](/docs/api/connection.html#connection_Connection-startSession).

```javascript
// Using Mongoose's default connection
const session = await mongoose.startSession();

// Using custom connection
const db = await mongoose.createConnection(mongodbUri);
const session = await db.startSession();
```

In practice, you should use either the [`session.withTransaction()` helper](https://mongodb.github.io/node-mongodb-native/3.2/api/ClientSession.html#withTransaction)
or Mongoose's `Connection#transaction()` function to run a transaction. The `session.withTransaction()` helper handles:

- Creating a transaction
- Committing the transaction if it succeeds
- Aborting the transaction if your operation throws
- Retrying in the event of a [transient transaction error](https://stackoverflow.com/questions/52153538/what-is-a-transienttransactionerror-in-mongoose-or-mongodb).

```javascript
[require:transactions.*withTransaction]
```

For more information on the `ClientSession#withTransaction()` function, please see
[the MongoDB Node.js driver docs](https://mongodb.github.io/node-mongodb-native/3.2/api/ClientSession.html#withTransaction).

Mongoose's `Connection#transaction()` function is a wrapper around `withTransaction()` that
integrates Mongoose change tracking with transactions.
For example, suppose you `save()` a document in a transaction that later fails.
The changes in that document are not persisted to MongoDB.
The `Connection#transaction()` function informs Mongoose change tracking that the `save()` was rolled back, and marks all fields that were changed in the transaction as modified.

```javascript
[require:transactions.*can save document after aborted transaction]
```

<h2 id="with-mongoose-documents-and-save"><a href="#with-mongoose-documents-and-save">With Mongoose Documents and <code>save()</code></a></h2>

If you get a [Mongoose document](/docs/documents.html) from [`findOne()`](/docs/api.html#findone_findOne)
or [`find()`](/docs/api.html#find_find) using a session, the document will
keep a reference to the session and use that session for [`save()`](/docs/api.html#document_Document-save).

To get/set the session associated with a given document, use [`doc.$session()`](/docs/api.html#document_Document-$session).

```javascript
[require:transactions.*save]
```

<h2 id="with-the-aggregation-framework"><a href="#with-the-aggregation-framework">With the Aggregation Framework</a></h2>

The `Model.aggregate()` function also supports transactions. Mongoose
aggregations have a [`session()` helper](/docs/api.html#aggregate_Aggregate-session)
that sets the [`session` option](/docs/api.html#aggregate_Aggregate-option).
Below is an example of executing an aggregation within a transaction.

```javascript
[require:transactions.*aggregate]
```

<h2 id="advanced-usage"><a href="#advanced-usage">Advanced Usage</a></h2>

Advanced users who want more fine-grained control over when they commit or abort transactions
can use `session.startTransaction()` to start a transaction:

```javascript
[require:transactions.*basic example]
```

You can also use `session.abortTransaction()` to abort a transaction:

```javascript
[require:transactions.*abort]
```
