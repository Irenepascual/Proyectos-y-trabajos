// src/app/layout.js o src/pages/_app.js (si usas Next.js 12 o anterior)
import { UserProvider } from '/src/app/context/UserContext';

export default function RootLayout({ children }) {
    return (
        <UserProvider>
            {children}
        </UserProvider>
    );
}
