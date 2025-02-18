const { MongoClient } = require('mongodb');

// URI de conexión: reemplaza <db_password> con tu contraseña y <db_name> con el nombre de tu base de datos
const uri = "mongodb+srv://874055:SisinfC08@postrecicoscluster.5y1z2.mongodb.net/postrecicosDATABASE?retryWrites=true&w=majority";

const client = new MongoClient(uri);

async function run() {
  try {
    // Conecta al clúster
    await client.connect();
    console.log("¡Conexión exitosa a MongoDB Atlas!");

    // Accede a la base de datos
    const database = client.db('postrecicosDATABASE'); // Cambia por tu base de datos
    const collection = database.collection('ingredientes'); // Cambia por tu colección

    // Prueba: Inserta un documento
    const result = await collection.insertOne({ nombre: 'Azúcar', cantidad: 5 });
    console.log(`Documento insertado con el ID: ${result.insertedId}`);
  } catch (err) {
    console.error("Error al conectar o ejecutar operaciones:", err);
  } finally {
    // Cierra la conexión
    await client.close();
  }
}

run().catch(console.dir);
