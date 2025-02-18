import React, { useState } from 'react';
import "/src/app/style-contacto.css";

/*
* Devuelve el componente Contacto, donde se carga
* el formulario a rellenar para guardar los 
* datos en la base de datos del nombre, el correo
* electrónico, el asunto y el mensaje en cuestión. 
*/
function Contacto() {
    const [formData, setFormData] = useState({
        nombre: '',
        correo: '',
        asunto: '',
        duda: '',
    });

    const handleChange = (e) => {
        setFormData({
            ...formData,
            [e.target.name]: e.target.value,
        });
    };

    const handleSubmit = async (e) => {
        e.preventDefault();
        console.log("Formulario enviado:", formData);

        try {
            const response = await fetch('/routes/contacto', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                },
                body: JSON.stringify(formData),
            });

            if (response.ok) {
                alert("Gracias por tu mensaje, nos pondremos en contacto pronto.");
                setFormData({
                    nombre: '',
                    correo: '',
                    asunto: '',
                    duda: '',
                });
            } else {
                console.error("Error en el envío del formulario");
            }
        } catch (error) {
            console.error("Error:", error);
        }
    };

    return (
        <div className="page-container">
            <div className="form-container"> 
                <form onSubmit={handleSubmit} className="formulario-contacto">
                    <div className="form-group">
                        <label htmlFor="nombre">Nombre:</label>
                        <input
                            type="text"
                            id="nombre"
                            name="nombre"
                            value={formData.nombre}
                            onChange={handleChange}
                            required
                            className="input-text"
                        />
                    </div>
                    <div className="form-group">
                        <label htmlFor="correo">Correo Electrónico:</label>
                        <input
                            type="email"
                            id="correo"
                            name="correo"
                            value={formData.correo}
                            onChange={handleChange}
                            required
                            className="input-text"
                        />
                    </div>
                    <div className="form-group">
                        <label htmlFor="asunto">Asunto:</label>
                        <input
                            type="text"
                            id="asunto"
                            name="asunto"
                            value={formData.asunto}
                            onChange={handleChange}
                            required
                            className="input-text"
                        />
                    </div>
                    <div className="form-group">
                        <label htmlFor="duda">Duda o Pregunta:</label>
                        <textarea
                            id="duda"
                            name="duda"
                            value={formData.duda}
                            onChange={handleChange}
                            required
                            className="textarea"
                        />
                    </div>
                    <button type="submit" className="submit-button">Enviar</button>
                </form>
            </div>
        </div>
    );
}

export default Contacto;
