"use client";
import { useForm } from "react-hook-form";
import { useRouter } from "next/navigation";
import { useState } from "react";

import "/src/app/style-register-login.css";

// Crea una pagina de inicio de sesion para un propietario con validaciones de los campos 
// y manejo de errores. Si el inicio de sesion es exitoso, el nombre de usuario se guarda 
// en el localStorage y el usuario es redirigido a la pagina de inicio con sesion. Si las 
// credenciales son incorrectas, se muestra un mensaje de error. Ademas, se ofrece 
// un enlace para redirigir a los usuarios que no tienen cuenta a la página de registro.
function LoginUsuarioPage() {
    const { register, handleSubmit, formState: { errors }, } = useForm();
    const router = useRouter();
    const [error, setError] = useState(null);

    const onSubmit = handleSubmit(async data => {
        console.log(data);

        const res = await fetch('/routes/loginPropietario', {
            method: "POST",
            body: JSON.stringify({
                nom_usuario: data.nom_usuario,
                contrasena: data.contrasena,
            }),
            headers: {
                "Content-Type": "application/json",
            },
        });

        console.log(res);

        if (!res.ok) {
            console.log(res.error);
            setError(res.error);
        } else {
            // Guardar el nombre de usuario en localStorage
            localStorage.setItem('nom_usuario', data.nom_usuario);

            // Login exitoso -> redirigir al usuario a la página home con sesión
            console.log('login exitoso');
            router.push("/home-con-sesionP");
            router.refresh();
        }
    });

return (
    <div className="login-page">
        <form onSubmit={onSubmit} className="login-form">

            {error && (
                <p className="error-message">
                    {error.message}
                </p>
            )}

            <h1>Iniciar Sesión Propietario</h1>

            <label>Nombre de usuario:
                <input 
                    type="text"
                    {...register("nom_usuario", {
                        required: {
                            value: true,
                            message: "El nombre de usuario es obligatorio",
                        },
                    })}
                    placeholder="miUsuario123"
                />
            </label>

            {errors.nom_usuario && (
                <span className="error-text">
                    {errors.nom_usuario.message}
                    <br />
                </span>
            )}

            <label>Contraseña:
                <input
                    type="password"
                    {...register("contrasena", {
                        required: {
                            value: true,
                            message: "La contraseña es obligatoria",
                        },
                    })}
                    placeholder="******"
                />
            </label>

            {errors.contrasena && (
                <span className="error-text">
                    {errors.contrasena.message}
                    <br />
                </span>
            )}

            <button className="login-button" type="submit">Iniciar Sesión</button>

            {/* Botón adicional debajo del botón de iniciar sesión */}
            <div className="register-link">
                <button
                    type="button"
                    onClick={() => router.push('/register/propietario')}
                >
                    ¿No tienes cuenta?
                </button>
            </div>
        </form>
    </div>
);
}

export default LoginUsuarioPage;
